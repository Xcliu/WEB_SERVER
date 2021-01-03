set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# generate socket_execption lib 
add_library(socket_exception socket_exception.cpp)
target_include_directories(socket_exception PUBLIC "$ENV{WEB_SERVER_PATH}/utils/debug_tool"
                                             PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )


