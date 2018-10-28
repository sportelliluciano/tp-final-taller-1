cmake_minimum_required(VERSION 2.8.11)

# Tomar los archivos fuente del cliente (todos los *.cpp)
file(GLOB libclientemodelo_srcs *.cpp)

# Generar una libería estática con todo el cliente EXCEPTO el main
add_library(clientemodelo ${libclientemodelo_srcs})
target_link_libraries(clientemodelo clientevideo)

# La idea de generar la librería es poder linkear más adelante con el
#  ejecutable de pruebas unitarias.
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
