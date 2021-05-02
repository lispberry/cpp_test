#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <boost/log/trivial.hpp>
#include <protocol.hpp>

using namespace boost;

void usage() {
    std::cout << "usage: client filename" << std::endl;
}

void readFileFromServer(const std::filesystem::path &path, uint16_t port = 7777) {
    std::ofstream out{path, std::ios::binary};
    asio::io_service service{};

    asio::ip::tcp::endpoint endpoint{asio::ip::make_address("127.0.0.1"), port};
    asio::ip::tcp::socket socket{service};

    try {
        socket.connect(endpoint);

        std::array<char, 8196> buffer{};
        server::FileHeader header{};
        asio::read(socket, asio::buffer(&header, sizeof(header)));

        boost::system::error_code ec;
        int64_t sz = header.size;
        while (ec != asio::error::eof && sz >= 0) {
            size_t bytesRead = asio::read(socket, asio::buffer(buffer), ec);
            sz -= bytesRead;
            if (ec && ec != asio::error::eof) {
                throw system::system_error{ec};
            }
            out.write(buffer.data(), bytesRead);
        }
    } catch (system::system_error &ex) {
        BOOST_LOG_TRIVIAL(error) << ex.what();
    }
}

int main(int argc, const char **argv) {
    if (argc < 2) {
        usage();
        return 0;
    }
    readFileFromServer(std::filesystem::current_path() / argv[1]);

    return 0;
}