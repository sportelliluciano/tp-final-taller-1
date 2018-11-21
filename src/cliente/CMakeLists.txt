cmake_minimum_required(VERSION 2.8.11)

# Tomar los archivos fuente del cliente (todos los *.cpp)
file(GLOB libcliente_srcs *.cpp)
file(GLOB eventos_cliente_src eventos/*.cpp)
list(APPEND libcliente_srcs ${eventos_cliente_src})
list(REMOVE_ITEM libcliente_srcs "main.cpp")

add_subdirectory(video)
add_subdirectory(sonido)
add_subdirectory(modelo)
add_subdirectory(lanzador)

# Generar una libería estática con todo el cliente EXCEPTO el main
add_library(libcliente ${libcliente_srcs})
target_link_libraries(libcliente lanzadorcliente conexion clientevideo clientemodelo clientesonido)
