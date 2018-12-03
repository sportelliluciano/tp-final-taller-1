cmake_minimum_required(VERSION 3.1.0)

find_package(Qt5 COMPONENTS Core Widgets CONFIG REQUIRED)
# http://doc.qt.io/qt-5/cmake-manual.html

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)


qt5_wrap_ui(UIS_HEADERS_EDITOR
  ./ui/Editor.ui
)

add_library(editor
  ${UIS_HEADERS_EDITOR}
  editor.cpp
  label_tab.cpp
  label_mapa.cpp
  dialogo_bienvenida.cpp
  generador_sprites.cpp
  manejador_json.cpp
  tabs.cpp
  mapa.cpp
)

# Use the Widgets module from Qt 5.
target_link_libraries(editor Qt5::Widgets)
