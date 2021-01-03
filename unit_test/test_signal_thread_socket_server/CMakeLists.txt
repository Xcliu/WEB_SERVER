set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# generate socket_execption lib 
add_executable(socket_exception_test socket_exception_test.cc)
target_link_libraries(socket_exception_test PUBLIC socket_exception)



