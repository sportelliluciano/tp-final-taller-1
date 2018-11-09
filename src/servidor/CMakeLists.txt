cmake_minimum_required(VERSION 2.8.11)


# Tomar los archivos fuente del cliente (todos los *.cpp)
file(GLOB libservidor_srcs *.cpp)
list(REMOVE_ITEM libservidor_srcs "main.cpp")

# Generar una libería estática con todo el cliente EXCEPTO el main
add_library(libservidor ${libservidor_srcs})
target_link_libraries(libservidor libcomun libmodelo)

# La idea de generar la librería es poder linkear más adelante con el
#  ejecutable de pruebas unitarias.
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
