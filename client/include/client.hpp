#ifndef CPP_TEST_CLIENT_HPP
#define CPP_TEST_CLIENT_HPP

#include <filesystem>
#include <cstdint>

namespace server {
void readFileFromServer(const std::filesystem::path &pathToSave, uint16_t port);
}

#endif//CPP_TEST_CLIENT_HPP
