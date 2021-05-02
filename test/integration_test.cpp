#include <server.hpp>
#include <client.hpp>
#include <filesystem>
#include <fstream>
#include <cstdio>
#include <string>
#include <thread>
#include <memory>

constexpr size_t FILE_SIZE = 128 * 1024 * 1024;

using path = std::filesystem::path;

path createTestFile(size_t size = FILE_SIZE) {
    auto path = std::filesystem::temp_directory_path() / "TEST.txt";
    std::ofstream f{path, std::ios::binary};
    for (size_t i = 0; i < size; i++) {
        f << '$';
    }
    return path;
}

void assertEqual(const path &x, const path &y) {
    auto readWhole = [](auto path) {
        std::ifstream in{path};
        std::string content{(std::istreambuf_iterator<char>{in}),
                             std::istreambuf_iterator<char>{in}};

        return content;
    };
    if (readWhole(x) != readWhole(y)) {
        exit(1);
    }
}

int main() {
    auto serverFile = createTestFile();
    auto clientFile = std::filesystem::temp_directory_path() / "TEST_OUT.txt";
    server::FileServer server{7777};
    server.start(serverFile);
    server::readFileFromServer(clientFile, 7777);
    assertEqual(serverFile, clientFile);
    server.stop();

    return 0;
}