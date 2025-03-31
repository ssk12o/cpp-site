cmake_minimum_required(VERSION 3.16)

add_library(generator generator.cpp)
target_include_directories(generator PUBLIC include)
target_link_libraries(generator PRIVATE utils PUBLIC person)
