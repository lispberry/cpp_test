#ifndef CPP_TEST_PROTOCOL_HPP
#define CPP_TEST_PROTOCOL_HPP

#include <cstdint>

namespace server {
struct FileHeader {
    // Note: little-endian
    int64_t size;
};

constexpr size_t MSG_BUFFER_SIZE = 64 * 1024;
}

#endif //CPP_TEST_PROTOCOL_HPP
