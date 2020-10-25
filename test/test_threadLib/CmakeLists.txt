cmake_minimum_required(VERSION 3.8)

project(test_wrie_read_lock VERSION 0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#get top dir for include and lib PATH
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT_DIR ${PARENT_DIR} DIRECTORY)
message(${PARENT_DIR})

set(lib_code_dir "${PARENT_DIR}/ThreadLib/")
set(source_addr "${lib_code_dir}/write_read_lock.cpp")

add_library(write_read_lock  ${source_addr} )

target_include_directories(write_read_lock  PUBLIC ${lib_code_dir})

set(main "test_read_write_lock.cpp")
add_executable(main ${main})
target_link_libraries(main PUBLIC write_read_lock)
target_include_directories(main PUBLIC ${lib_code_dir})


