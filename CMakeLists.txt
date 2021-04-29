cmake_minimum_required(VERSION 3.19)
project(cpp_test)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.71.0 REQUIRED COMPONENTS system)
if(Boost_FOUND)
    include_directories(${BOOST_INCLUDE_DIRS})
    add_executable(server server.cpp)
    target_link_libraries(server ${Boost_LIBRARIES})
endif()
