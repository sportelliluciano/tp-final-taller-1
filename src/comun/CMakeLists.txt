cmake_minimum_required(VERSION 2.8.11)

file(GLOB libcomun_srcs *.cpp)

add_library(libcomun ${libcomun_srcs})

