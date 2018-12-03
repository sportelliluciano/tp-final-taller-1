cmake_minimum_required(VERSION 3.1.0)

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
    ./lanzador_en_sala.ui
    ./lanzador_elegir_sala.ui
)

qt5_add_resources(UIS_RC
    ${PROJECT_SOURCE_DIR}/data/rc-cliente/recursos.qrc
)

add_library(lanzadorcliente
    ${UIS_RC}
    ${UIS_HEADERS}
    jugador.cpp
    lanzador_en_sala.cpp
    lanzador_elegir_sala.cpp
    lanzador.cpp
    ventana_principal.cpp
)

target_link_libraries(lanzadorcliente Qt5::Widgets clientered)
