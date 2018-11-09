cmake_minimum_required(VERSION 2.8.11)


file(GLOB libservidor_srcs *.cpp)
list(REMOVE_ITEM libservidor_srcs "main.cpp")


add_library(libservidor ${libservidor_srcs})
target_link_libraries(libservidor libcomun)
