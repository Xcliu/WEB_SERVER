set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(reflection_test  relection_test.cpp)
target_link_libraries(reflection_test PUBLIC reflect_lib)
