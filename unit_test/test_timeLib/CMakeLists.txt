cmake_minimum_required(VERSION 3.8)

project(test_wrie_read_lock VERSION 0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(test_function_timeout test_function_timeout.cpp)
add_executable(test_timer test_timer.cpp)
