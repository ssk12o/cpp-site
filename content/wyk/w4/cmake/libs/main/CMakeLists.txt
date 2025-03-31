cmake_minimum_required(VERSION 3.16)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE generator)
