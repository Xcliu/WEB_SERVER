cmake_minimum_required(VERSION 3.8)

project(test_wrie_read_lock VERSION 0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(test_read_write_lock test_read_write_lock.cpp)
target_link_libraries(test_read_write_lock PUBLIC write_read_lock)

add_executable(test_thread_safe_queue test_thread_safe_queue.cpp)

add_executable(test_thread_pool test_thread_pool.cpp)
