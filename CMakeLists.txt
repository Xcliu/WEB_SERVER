cmake_minimum_required(VERSION 3.8)
project(reflect_lib VERSION 0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories($ENV{WEB_SERVER_PATH})
link_directories($ENV{WEB_SERVER_PATH})

add_subdirectory(socket_lib/)
