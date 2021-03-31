cmake_minimum_required(VERSION 3.8)
project(test_signal_thread_socket_server_stream VERSION 0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC 
    server_socket_stream server_socket socket_exception socket_address)

add_executable(client_main client_main.cpp)
