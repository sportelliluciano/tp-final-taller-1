cmake_minimum_required(VERSION 2.8.11)

# Código general del mini-framework
add_library(libtesting testing.cpp)
enable_testing()

foreach(ejecutable servidor cliente editor modelo)
    # Cargar todos los archivos de pruebas unitarias
    file(GLOB unit_test_srcs "${ejecutable}/*.h")

    # Cargar las variables para generar test-main-*.cpp
    set(EJECUTABLE_A_TESTEAR "${ejecutable}")
    set(TEST_INCLUDES "")
    if(unit_test_srcs)
        foreach(archivo IN LISTS unit_test_srcs)
            set(TEST_INCLUDES "${TEST_INCLUDES}#include \"${archivo}\"\n")
        endforeach(archivo)
    endif(unit_test_srcs)

    # Generar test-main.h
    configure_file("test-main.cpp.in" "test-main-${ejecutable}.cpp")
    add_executable(${ejecutable}-unit-tests test-main-${ejecutable}.cpp)
    target_link_libraries(${ejecutable}-unit-tests libtesting lib${ejecutable})
    add_test(${ejecutable}-unit-tests ${ejecutable}-unit-tests)

endforeach(ejecutable)
