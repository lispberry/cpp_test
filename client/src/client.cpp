#include <fstream>
#include <filesystem>
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <boost/log/trivial.hpp>
#include <protocol.hpp>
#include <client.hpp>

using namespace boost;

namespace server {
void readFileFromServer(const std::filesystem::path &path, uint16_t port) {
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
}