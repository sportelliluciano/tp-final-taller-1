cmake_minimum_required(VERSION 3.1.0)
# Qt5 pide minimo v3.1.0
project(editor)

# Busca el módulo Widgets de la biblioteca Qt5
# en particular toma la configuración de /usr/lib/x86_64-linux-gnu/cmake/Qt5/Qt5WidgetsConfig.cmake

# find_package(Qt5 COMPONENTS Widgets Core <...>) es una forma más feliz de
# agregar paquetes, ver https://blog.kitware.com/cmake-finding-qt5-the-right-way/
find_package(Qt5Widgets)

# Esta macro toma cada uno los parámetros y ejecuta *uic* sobre ellos
# La aplicacion uic genera headers con la definición de los widgets
# a partir de un archivo .ui
# Los nombres de los headers generados se guardan en la variable pasada
# como primer parámetro.
# No, no los genera con la carpeta como prefijo.
qt5_wrap_ui(UIS_HEADERS
        ${PROJECT_SOURCE_DIR}/ui/Editor.ui
)

# Agrego la carpeta con los headers autogenerados a los includes
message("Agregando '${PROJECT_BINARY_DIR}' como directorio de includes")
include_directories(${PROJECT_BINARY_DIR})

message("Headers generados: ${UIS_HEADERS}")

add_executable(editor
  Editor.cpp
  main.cpp
  LabelTab.cpp
  LabelMapa.cpp
  DialogoBienvenida.cpp
  GeneradorSprites.cpp
  ManejadorJson.cpp
  Tabs.cpp
  Mapa.cpp
  ${UIS_HEADERS})

# Use the Widgets module from Qt 5.
target_link_libraries(editor Qt5::Widgets)
