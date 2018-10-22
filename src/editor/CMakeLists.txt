cmake_minimum_required(VERSION 3.1.0)
# Qt5 pide minimo v3.1.0
project(editor)

# Busca el módulo Widgets de la biblioteca Qt5
# en particular toma la configuración de /usr/lib/x86_64-linux-gnu/cmake/Qt5/Qt5WidgetsConfig.cmake

# find_package(Qt5 COMPONENTS Widgets Core <...>) es una forma más feliz de
# agregar paquetes, ver https://blog.kitware.com/cmake-finding-qt5-the-right-way/
find_package(Qt5Widgets)

# Linkea libQt5Widgets.so al ejecutable
#target_link_libraries(editor Qt5::Widgets)

file(GLOB libeditor_srcs *.cpp)
list(REMOVE_ITEM libeditor_srcs "main.cpp")

add_library(libeditor ${libeditor_srcs})
target_link_libraries(libeditor libcomun Qt5::Widgets)
