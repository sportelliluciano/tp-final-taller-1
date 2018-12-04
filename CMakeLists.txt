cmake_minimum_required(VERSION 3.1)
project(dune-remake)

set (CMAKE_BUILD_TYPE Debug)
option(CL_USAR_ASAN "Compilar el cliente con address sanitizer" OFF)
option(SV_DEPURAR_TIEMPOS "Activar la depuración de tiempos en el servidor" OFF)
option(DEPURAR_RED "Activar la depuración de mensajes de red" OFF)

# Carpeta donde se encuentra la configuración del servidor
set(SV_CONFIG_DIR "${CMAKE_INSTALL_PREFIX}/etc/dune-remake")

#Carpeta donde se encuentran los datos del editor
set(EDITOR_CONFIG_DIR "${CMAKE_INSTALL_PREFIX}/etc/dune-remake")

# Carpetas donde se encuentran los datos del cliente
set(ASSETS_INSTALL_DIR share/dune-remake)
set(ASSETS_DIR ${CMAKE_INSTALL_PREFIX}/${ASSETS_INSTALL_DIR})
set(TERRAIN_7Z "${PROJECT_BINARY_DIR}/terrain.7z")
set(SPRITES_7Z "${PROJECT_BINARY_DIR}/sprites.7z")
set(RENOMBRADOR "${PROJECT_SOURCE_DIR}/data/renombrar-imagenes.py")

find_program(CHMOD chmod)
find_program(RM rm)
find_program(PYTHON3 python3)
find_program(UTIL_7Z 7z)

if(NOT EXISTS "${TERRAIN_7Z}")
 message(FATAL_ERROR "Debe copiar el archivo terrain.7z provisto por la cátedra en ${TERRAIN_7Z}")
endif(NOT EXISTS "${TERRAIN_7Z}")

if(NOT EXISTS "${SPRITES_7Z}")
 message(FATAL_ERROR "Debe copiar el archivo sprites.7z provisto por la cátedra en ${SPRITES_7Z}")
endif(NOT EXISTS "${SPRITES_7Z}")

if(NOT EXISTS "${RENOMBRADOR}")
 message(FATAL_ERROR "No se encontró el renombrador de sprites. Debería estar en ${RENOMBRADOR}")
endif(NOT EXISTS "${RENOMBRADOR}")

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

if(DEPURAR_RED)
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEPURACION_RED=1")
endif(DEPURAR_RED)

# Todos los #include son relativos a src
include_directories(src)
# Archivos generados por CMake
include_directories(${PROJECT_BINARY_DIR}/src)

add_subdirectory(src/comun)
add_subdirectory(src/cliente)
add_subdirectory(src/modelo)
add_subdirectory(src/servidor)
add_subdirectory(src/editor)

# Generar el cliente
add_executable(dune-remake src/cliente/main.cpp)
if(CL_USAR_ASAN)
 target_link_libraries(dune-remake asan cliente)
else(CL_USAR_ASAN)
 target_link_libraries(dune-remake cliente)
endif(CL_USAR_ASAN)

# Generar el servidor
add_executable(dune-remake-servidor src/servidor/main.cpp)
target_link_libraries(dune-remake-servidor servidor)

# Generar el editor
add_executable(dune-remake-editor src/editor/main.cpp)
target_link_libraries(dune-remake-editor editor)

# Copiar binarios
install(TARGETS dune-remake dune-remake-servidor dune-remake-editor DESTINATION bin)
# Copiar configuración inicial y mapas base del servidor
install(FILES data/servidor/config.json
              data/servidor/edificios.json
              data/servidor/ejercitos.json
        DESTINATION ${SV_CONFIG_DIR})
file(GLOB SV_MAPAS_BASE data/servidor/mapas/*.json)
install(FILES ${SV_MAPAS_BASE} DESTINATION ${SV_CONFIG_DIR}/mapas)
# Copiar assets
install(FILES data/assets/fuente.ttf DESTINATION ${ASSETS_INSTALL_DIR})
file(GLOB_RECURSE ASSETS_SONIDOS data/assets/sonidos/*)
install(FILES ${ASSETS_SONIDOS} DESTINATION ${ASSETS_INSTALL_DIR}/sonidos)
file(GLOB_RECURSE ASSETS_IMAGENES data/assets/imagenes/*)
install(FILES ${ASSETS_IMAGENES} DESTINATION ${ASSETS_INSTALL_DIR}/imagenes)
# Extraer sprites de la cátedra, renombrarlos y ubicarlos en la carpeta indicada
install(CODE "execute_process(COMMAND ${RM} -rf \"${ASSETS_DIR}/terrenos\")")
install(CODE "execute_process(COMMAND ${RM} -rf \"${ASSETS_DIR}/sprites\")")
install(CODE "execute_process(COMMAND ${UTIL_7Z} e \"${TERRAIN_7Z}\" terrain/* -y \"-o${ASSETS_DIR}/terrenos\")")
install(CODE "execute_process(COMMAND ${UTIL_7Z} e \"${SPRITES_7Z}\" imgs/imgs/* -y \"-o${ASSETS_DIR}/sprites\")")
install(CODE "message(\"Procesando sprites\")")
install(CODE "execute_process(COMMAND ${PYTHON3} \"${RENOMBRADOR}\" \"${ASSETS_DIR}/sprites\" OUTPUT_FILE renombrar.sh)")
install(CODE "execute_process(COMMAND ${CHMOD} +x renombrar.sh)")
install(CODE "execute_process(COMMAND ./renombrar.sh)")
install(CODE "execute_process(COMMAND ${RM} renombrar.sh)")
install(CODE "message(\"Hecho.\")")
