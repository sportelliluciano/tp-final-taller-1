cmake_minimum_required(VERSION 2.8.11)

# Tomar los archivos fuente del cliente (todos los *.cpp)
file(GLOB_RECURSE libclientevideo_srcs *.cpp)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS}
                    ${SDL2IMAGE_INCLUDE_DIRS}
                    ${SDL2TTF_INCLUDE_DIRS})

# Generar una libería estática con todo el cliente EXCEPTO el main
add_library(clientevideo ${libclientevideo_srcs})
target_link_libraries(clientevideo ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES})
