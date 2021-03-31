cmake_minimum_required(VERSION 3.8)
project(test_signal_thread_socket_server_datagram VERSION 0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(server_main server_main.cpp)
target_link_libraries(server_main PUBLIC 
    server_socket_datagram server_socket socket_exception socket_address)

add_executable(client_main client_main.cpp)
target_link_libraries(client_main PUBLIC 
    server_socket_datagram server_socket socket_exception socket_address)


add_executable(server_main_ipv6 server_main_ipv6.cpp)
target_link_libraries(server_main_ipv6 PUBLIC 
    server_socket_datagram server_socket socket_exception socket_address)


add_executable(client_main_ipv6 client_main_ipv6.cpp)
target_link_libraries(client_main_ipv6 PUBLIC 
    server_socket_datagram server_socket socket_exception socket_address)



