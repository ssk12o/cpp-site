cmake_minimum_required(VERSION 3.16)

add_executable(test test.cpp)
target_link_libraries(test PRIVATE utils)
