#ifndef CPP_TEST_SERVER_HPP
#define CPP_TEST_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <string>
#include <filesystem>

namespace server {
using namespace boost;
using namespace boost::asio;

class FileServer {
public:
    explicit FileServer(uint16_t port);
    ~FileServer();
    void start(const std::filesystem::path &path);
private:
    io_service m_service{};
    ip::tcp::acceptor m_acceptor;
};
}

#endif//CPP_TEST_SERVER_HPP
