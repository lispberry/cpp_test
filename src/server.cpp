#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <iostream>
#include <fstream>
#include <thread>
#include <filesystem>
#include <boost/log/trivial.hpp>
#include <server.hpp>

namespace server {
using namespace boost;
using namespace std::chrono_literals;

FileServer::FileServer(uint16_t port)
    : m_acceptor{m_service, ip::tcp::endpoint{ip::tcp::v4(), port}}
    {}

void FileServer::start(const std::filesystem::path &path) {
    for (;;) {
        try {
            auto socket = m_acceptor.accept();
            std::thread([path, socket = std::move(socket)]() mutable {
                std::ifstream in{path};
                std::array<char, 8196> buffer{};
                try {
                    FileHeader header{};
                    header.size = std::filesystem::file_size(path);

                    asio::write(socket, asio::buffer(&header, sizeof(header)));
                    while (in.read(buffer.data(), buffer.size())) {
                        asio::write(socket, asio::buffer(buffer));
                    }
                } catch (std::exception &ex) {
                    socket.close();
                    BOOST_LOG_TRIVIAL(info) << ex.what();
                }
            }).detach();
        } catch (std::exception &ex) {
            BOOST_LOG_TRIVIAL(info) << ex.what();
        }
    }
}

FileServer::~FileServer() {}

}

int main() {
    server::FileServer server(7777);
    server.start(std::filesystem::current_path() / "test.txt");

    return 0;
}