cmake_minimum_required(VERSION 3.17)
project(cpp_test)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(protocol)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(test)