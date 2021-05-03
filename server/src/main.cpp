#include <iostream>
#include <string>
#include <server.hpp>

void usage() {
    std::cout << "usage ./server filename port";
}

int main(int argc, char **argv) {
    if (argc < 3) {
        usage();
        return 0;
    }
    server::FileServer server{(uint16_t)atoi(argv[2])};
    server.start(std::filesystem::current_path() / argv[1]);
    server.wait();

    return 0;
}