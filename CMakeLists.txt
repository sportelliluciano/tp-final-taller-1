cmake_minimum_required(VERSION 2.8.11)

project(dune-remake)

# Modo DEBUG
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O0 -fno-inline" CACHE STRING "" FORCE)
#### FIN MODO DEBUG

# Flags de compilación a utilizar **para** el source.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -Wno-sign-conversion")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow -Wpointer-arith")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunreachable-code -Wformat=2")

# Todos los #include son relativos a src
include_directories(src)

add_subdirectory(src)

# Generar el cliente
add_executable(cliente src/cliente/main.cpp)
target_link_libraries(cliente libcliente)

# Generar el servidor
add_executable(servidor src/servidor/main.cpp)
target_link_libraries(servidor libservidor)
