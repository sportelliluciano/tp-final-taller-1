cmake_minimum_required(VERSION 2.8.11)

file(GLOB_RECURSE libclientered_src *.cpp)

add_library(clientered ${libclientered_src})
target_link_libraries(clientered comun)
