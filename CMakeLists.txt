cmake_minimum_required(VERSION 3.1)

project(dune-remake)

set (CMAKE_BUILD_TYPE Debug)
option(CLIENTE_USAR_ASAN "Compilar el cliente con address sanitizer" OFF)

# Modo DEBUG
set (CMAKE_CXX_FLAGS_DEBUG "-ggdb -O0 -fno-inline")

# Estándar C++11
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF) # -std=c++11 en lugar de -std=gnu++11

# Flags de compilación a utilizar en general.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -Wno-sign-conversion")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow -Wpointer-arith")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunreachable-code -Wformat=2")

# Todos los #include son relativos a src
include_directories(src)

add_subdirectory(src/comun)
add_subdirectory(src/cliente)
add_subdirectory(src/modelo)
add_subdirectory(src/servidor)

# Generar el cliente
add_executable(cliente src/cliente/main.cpp)
if(CLIENTE_USAR_ASAN)
 target_link_libraries(cliente asan libcliente)
else(CLIENTE_USAR_ASAN)
 target_link_libraries(cliente libcliente)
endif(CLIENTE_USAR_ASAN)

# Generar el servidor
add_executable(servidor src/servidor/main.cpp)
target_link_libraries(servidor libservidor)
