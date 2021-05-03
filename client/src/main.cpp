#include <client.hpp>
#include <filesystem>
#include <iostream>

void usage() {
    std::cout << "usage: client filename port" << std::endl;
}

int main(int argc, const char **argv) {
    if (argc < 3) {
        usage();
        return 0;
    }
    server::readFileFromServer(std::filesystem::current_path() / argv[1], atoi(argv[2]));

    return 0;
}