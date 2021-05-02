#include <client.hpp>
#include <filesystem>
#include <iostream>

void usage() {
    std::cout << "usage: client filename" << std::endl;
}

int main(int argc, const char **argv) {
    if (argc < 2) {
        usage();
        return 0;
    }
    server::readFileFromServer(std::filesystem::current_path() / argv[1]);

    return 0;
}