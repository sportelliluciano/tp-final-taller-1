cmake_minimum_required(VERSION 2.8.11)

file(GLOB_RECURSE comun_srcs *.cpp)

add_library(comun ${comun_srcs})
