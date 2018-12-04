cmake_minimum_required(VERSION 2.8.11)

file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/src/servidor")
configure_file(
    "${PROJECT_SOURCE_DIR}/src/servidor/config.h.in"
    "${PROJECT_BINARY_DIR}/src/servidor/config.h"
)

file(GLOB libservidor_srcs *.cpp)
list(REMOVE_ITEM libservidor_srcs "main.cpp")

if(SV_DEPURAR_TIEMPOS)
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEPURAR_TIEMPOS")
endif(SV_DEPURAR_TIEMPOS)

add_library(servidor ${libservidor_srcs})
target_link_libraries(servidor pthread comun modelo)
