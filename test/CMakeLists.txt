add_executable(test_file_send integration_test.cpp)
target_link_libraries(test_file_send server_lib client_lib)

add_test(NAME integration_test
        COMMAND test_file_send)