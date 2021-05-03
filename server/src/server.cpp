#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <boost/endian.hpp>
#include <iostream>
#include <fstream>
#include <thread>
#include <filesystem>
#include <boost/log/trivial.hpp>
#include <server.hpp>
#include <protocol.hpp>

namespace server {
using namespace boost;
using namespace std::chrono_literals;

const uint16_t THREAD_POOL_SIZE = std::thread::hardware_concurrency() * 2;

FileServer::FileServer(uint16_t port)
    : m_acceptor{m_service, ip::tcp::endpoint{ip::tcp::v4(), port}},
      m_workers{THREAD_POOL_SIZE}
    {}

void FileServer::start(const std::filesystem::path &path) {
    m_acceptor.async_accept([path, this](auto ec, auto socket) {
        BOOST_LOG_TRIVIAL(info) << "Accepted a new connection";
        acceptConnection(path, std::move(socket));
    });
    m_timeout.async_wait([this](auto ec) { handleTimeout(); });
    m_acceptor_thread = std::thread([this]() { m_service.run(); });
}

void FileServer::handleTimeout() {
    if (m_exit_cond.load()) {
        BOOST_LOG_TRIVIAL(info) << "Shut down after timeout";
        m_service.stop();
    }
    m_timeout.async_wait([this](auto ec) { handleTimeout(); });
}

void FileServer::acceptConnection(const std::filesystem::path &path, ip::tcp::socket socket) {
    asio::post(m_workers,[path, socket = std::move(socket)]() mutable {
        std::ifstream in{path, std::ios::binary};
        std::array<char, MSG_BUFFER_SIZE> buffer{};
        try {
            FileHeader header{};
            header.size = endian::native_to_little(std::filesystem::file_size(path));

            asio::write(socket, asio::buffer(&header, sizeof(header)));
            size_t bytesSent = 0;
            while (in.read(buffer.data(), buffer.size()) || in.gcount() > 0) {
                bytesSent += in.gcount();
                asio::write(socket, asio::buffer(buffer.data(), in.gcount()));
            }
            BOOST_LOG_TRIVIAL(info) << "Sent bytes: " << bytesSent;
        } catch (std::exception &ex) {
            BOOST_LOG_TRIVIAL(error) << ex.what();
        }
        socket.close();
    });

    m_acceptor.async_accept([path, this](auto ec, auto socket) {
        acceptConnection(path, std::move(socket));
    });
}

void FileServer::stop() {
    BOOST_LOG_TRIVIAL(info) << "Started shutting down";
    shutdown();
}

void FileServer::wait() {
    if (m_acceptor_thread.joinable()) {
        m_acceptor_thread.join();
    }
    m_workers.join();
}

FileServer::~FileServer() {
    shutdown();
}

void FileServer::shutdown(){
    m_exit_cond.store(true);
    if (m_acceptor_thread.joinable()) {
        m_acceptor_thread.join();
    }
    m_workers.join();
}
}