cmake_minimum_required(VERSION 2.8.11)

# Tomar los archivos fuente del cliente (todos los *.cpp)
file(GLOB libcliente_srcs *.cpp)
list(REMOVE_ITEM libcliente_srcs "main.cpp")

# Generar una libería estática con todo el cliente EXCEPTO el main
add_library(libcliente ${libcliente_srcs})
target_link_libraries(libcliente libcomun)

# La idea de generar la librería es poder linkear más adelante con el
#  ejecutable de pruebas unitarias.
