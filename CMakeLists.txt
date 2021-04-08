cmake_minimum_required(VERSION 3.8)
project(reflect_lib VERSION 0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -pthread")

include_directories($ENV{WEB_SERVER_PATH})
link_directories($ENV{WEB_SERVER_PATH})

add_subdirectory(socket_lib/)
add_subdirectory(unit_test/test_socket/test_stream_socket/)
add_subdirectory(utils/ThreadLib)
add_subdirectory(unit_test/test_threadLib)
add_subdirectory(unit_test/test_timeLib)