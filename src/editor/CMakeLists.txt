cmake_minimum_required(VERSION 2.8.11)


file(GLOB libeditor_srcs *.cpp)
list(REMOVE_ITEM libeditor_srcs "main.cpp")


add_library(libeditor ${libeditor_srcs})

