set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(socket_exception socket_exception.cpp)

add_library(socket_address socket_address.cpp )

add_library(server_socket server_socket.cpp)

add_library(server_socket_stream server_socket_stream.cpp)
                                            
