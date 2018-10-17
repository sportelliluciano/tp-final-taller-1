cmake_minimum_required(VERSION 2.8.11)

# Tomar los archivos fuente del cliente (todos los *.cpp)
file(GLOB libcliente_srcs *.cpp)
list(REMOVE_ITEM libcliente_srcs "main.cpp")

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

# Generar una libería estática con todo el cliente EXCEPTO el main
add_library(libcliente ${libcliente_srcs})
target_link_libraries(libcliente libcomun ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

# La idea de generar la librería es poder linkear más adelante con el
#  ejecutable de pruebas unitarias.
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
