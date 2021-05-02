#ifndef CPP_TEST_SERVER_HPP
#define CPP_TEST_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <string>
#include <filesystem>
#include <atomic>

namespace server {
using namespace boost;
using namespace boost::asio;

class FileServer {
public:
    explicit FileServer(uint16_t port);
    ~FileServer();
    void start(const std::filesystem::path &path);
    void wait();
    void stop();
private:
    void acceptConnection(const std::filesystem::path &path, ip::tcp::socket socket);
    void handleTimeout();
private:
    io_service m_service{};
    ip::tcp::acceptor m_acceptor;
    deadline_timer m_timeout{m_service, posix_time::seconds{15}};
    std::thread m_acceptor_thread{};
    std::atomic<bool> m_exit_cond{false};
};
}

#endif//CPP_TEST_SERVER_HPP
