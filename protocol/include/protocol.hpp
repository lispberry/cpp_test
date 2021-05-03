#ifndef CPP_TEST_PROTOCOL_HPP
#define CPP_TEST_PROTOCOL_HPP

#include <cstdint>

namespace server {
struct FileHeader {
    // Note: little-endian
    int64_t size;
};
}

#endif //CPP_TEST_PROTOCOL_HPP
