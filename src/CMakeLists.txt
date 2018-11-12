cmake_minimum_required(VERSION 2.8.11)

# Flags de compilación a utilizar **para** el source.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -Wno-sign-conversion")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow -Wpointer-arith")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunreachable-code -Wformat=2")

add_subdirectory(comun)
#add_subdirectory(conexion)
add_subdirectory(modelo)
add_subdirectory(servidor)
#add_subdirectory(cliente)
add_subdirectory(editor)


