cmake_minimum_required(VERSION 2.8.11)

# Tomar los archivos fuente del cliente (todos los *.cpp)
file(GLOB libmodelo_srcs *.cpp)

# Generar una libería estática con todo el cliente EXCEPTO el main
add_library(modelo ${libmodelo_srcs})

