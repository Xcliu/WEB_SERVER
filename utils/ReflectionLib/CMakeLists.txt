cmake_minimum_required(VERSION 3.8)

project(reflect_lib VERSION 0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(reflect_lib  ${CMAKE_CURRENT_SOURCE_DIR}/reflection.cpp)
target_include_directories(reflect_lib  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
