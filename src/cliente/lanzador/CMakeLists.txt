cmake_minimum_required(VERSION 2.8.11)

find_package(Qt5 COMPONENTS Core Widgets CONFIG REQUIRED)

# http://doc.qt.io/qt-5/cmake-manual.html

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
#set(CMAKE_AUTOUIC ON)

qt5_wrap_ui(UIS_HEADERS
    ./lanzador.ui
)

qt5_add_resources(UIS_RC
    ${PROJECT_SOURCE_DIR}/data/rc-cliente/recursos.qrc
)

add_library(lanzadorcliente
    ${UIS_RC}
    ${UIS_HEADERS}
    estado_en_sala.cpp
    estado_conectado.cpp
    estado_desconectado.cpp
    estado_lanzador.cpp
    estado_lanzador.h
    lanzador.cpp
)

target_link_libraries(lanzadorcliente Qt5::Widgets)
