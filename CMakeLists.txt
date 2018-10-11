cmake_minimum_required(VERSION 2.8.11)

project(dune-remake)

include_directories(.)
include_directories(src)

add_subdirectory(src)
add_subdirectory(test)

# Generar el cliente
add_executable(cliente src/cliente/main.cpp)
target_link_libraries(cliente libcliente)

# Generar el servidor
add_executable(servidor src/servidor/main.cpp)
target_link_libraries(servidor libservidor)

# Generar el editor
add_executable(editor src/editor/main.cpp)
target_link_libraries(editor libeditor)
