cmake_minimum_required(VERSION 2.8.11)

# Tomar los archivos fuente del cliente (todos los *.cpp)
file(GLOB libclientesonido_srcs *.cpp)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS}
                    ${SDL2MIXER_INCLUDE_DIRS})

# Generar una libería estática con todo el cliente EXCEPTO el main
add_library(clientesonido ${libclientesonido_srcs})
target_link_libraries(clientesonido ${SDL2_LIBRARIES} ${SDL2MIXER_LIBRARIES})

# La idea de generar la librería es poder linkear más adelante con el
#  ejecutable de pruebas unitarias.
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
