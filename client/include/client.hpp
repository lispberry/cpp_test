#ifndef CPP_TEST_CLIENT_HPP
#define CPP_TEST_CLIENT_HPP

#include <filesystem>
#include <cstdint>

namespace server {
void readFileFromServer(const std::filesystem::path &path, uint16_t port = 7777);
}

#endif//CPP_TEST_CLIENT_HPP
