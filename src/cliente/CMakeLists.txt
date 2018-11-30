cmake_minimum_required(VERSION 2.8.11)

file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/src/cliente")
configure_file(
    "${PROJECT_SOURCE_DIR}/src/cliente/config.h.in"
    "${PROJECT_BINARY_DIR}/src/cliente/config.h"
)

# Tomar los archivos fuente del cliente (todos los *.cpp)
file(GLOB libcliente_srcs *.cpp)
file(GLOB eventos_cliente_src eventos/*.cpp)
list(APPEND libcliente_srcs ${eventos_cliente_src})
list(REMOVE_ITEM libcliente_srcs "main.cpp")

if(CL_USAR_ASAN)
 set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
endif(CL_USAR_ASAN)

add_subdirectory(video)
add_subdirectory(sonido)
add_subdirectory(modelo)
add_subdirectory(lanzador)

# Generar una libería estática con todo el cliente EXCEPTO el main
add_library(cliente ${libcliente_srcs})
target_link_libraries(cliente
    comun
    clientevideo
    clientesonido
    clientemodelo
    lanzadorcliente
)
