set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_library(reflect_lib  reflection.cpp)
target_include_directories(reflect_lib  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
