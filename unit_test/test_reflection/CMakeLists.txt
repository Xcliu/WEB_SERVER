cmake_minimum_required(VERSION 3.8)

project(reflect_lib VERSION 0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(WEB_SERVER_PATH "/root/lxc/git_project/WEB_SERVER")
add_library(reflection ${WEB_SERVER_PATH}/utils/ReflectionLib/reflection.cpp)

add_executable(reflection_test  ${CMAKE_CURRENT_SOURCE_DIR}/relection_test.cpp)
target_include_directories(reflection_test  PUBLIC ${WEB_SERVER_PATH}/utils/ReflectionLib/)
target_link_libraries(reflection_test PUBLIC reflection)