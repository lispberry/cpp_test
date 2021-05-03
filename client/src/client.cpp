#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <boost/log/trivial.hpp>
#include <boost/endian.hpp>
#include <protocol.hpp>
#include <client.hpp>

using namespace boost;

namespace server {

template <typename Func>
auto runWithTime(Func action) {
    using clock = std::chrono::high_resolution_clock;
    auto start = clock::now();
    action();
    auto end = clock::now();
    return end - start;
}

void readFileFromServer(const std::filesystem::path &pathToSave, uint16_t port) {
    std::ofstream out{pathToSave, std::ios::binary};
    asio::io_service service{};

    asio::ip::tcp::endpoint endpoint{asio::ip::make_address("127.0.0.1"), port};
    asio::ip::tcp::socket socket{service};

    auto delay = runWithTime([&] {
        try {
            socket.connect(endpoint);
            std::array<char, 8196> buffer{};
            server::FileHeader header{};
            asio::read(socket, asio::buffer(&header, sizeof(header)));

            boost::system::error_code ec;
            int64_t sz = endian::little_to_native(header.size);
            while (ec != asio::error::eof && sz >= 0) {
                size_t bytesRead = asio::read(socket, asio::buffer(buffer), ec);
                sz -= bytesRead;
                if (ec && ec != asio::error::eof) {
                    throw system::system_error{ec};
                }
                out.write(buffer.data(), bytesRead);
            }
            if (sz > 0) {
                std::cout << "Couldn't download the whole file" << std::endl;
            }
        } catch (system::system_error &ex) {
            std::cout << "Couldn't download the file" << std::endl;
            BOOST_LOG_TRIVIAL(error) << ex.what();
        }
    });

    std::cout << "Transmission delay is: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(delay).count()
              << "ms"
              << std::endl;
}
}