cmake_minimum_required(VERSION 3.16)

add_library(utils utils.cpp impl.cpp)
target_include_directories(utils PUBLIC include)
