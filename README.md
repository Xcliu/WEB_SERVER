# WEB_SERVER

## Introduction to this project 

The web server named SONNIE is mainly wrote by c++, and the c-style UNIX socket API is also widely used. Up to now, the sample communication based on TCP and UDP is supported. The sample exception handling mechnism is added. In addition, some useful utils is also developed, such as the the thread pool and the reflection lib which implement the sample refection machnism based on C++11. The multi-threding, coroutine and IO-multiplexing, which are the key techonlogies for concurrency, are undering developing. Finally, I will try to make SONNIE support CGI(common gateway interface).

## How to use it
1.write the top level CMakeLists.txt file which contain the sub-CMakelists.txt for the target lib or binary.

example:
``` cmake
cmake_minimum_required(VERSION 3.8)
project(reflect_lib VERSION 0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(utils/ReflectionLib)
add_subdirectory(unit_test/test_reflection)
```
2.generate makefile (or other native tools) and complie your project deifned in the top level CMakeLists.txt

complie your project:
``` shell
./project_tool.sh complie
```
remove the cached cmake build directory:
``` shell
./project_tool.sh clean_cache 
```

## The architecture of this project
+++ d: **BUILD**(may be not exist) contain the cmake cache file

+++ d: **sand_box** contain casual codes

+++ d: **socket_lib** contain the socket lib based on  UNIX socket API

+++ d: **unit_test** testing code for other libs 

+++ d: **utils** contains some useful tools

+++ f: **CMakeLists.txt** top level cmake file used to define how to gennerate your lib or bin

+++ f: **project_tool.sh** shell script used to complie your project defined in top level cmake 
