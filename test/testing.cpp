#include "test/testing.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

/************************ Implementación clase Testing ************************/
Testing& Testing::obtener_instancia() {
    static Testing testing;
    return testing;
}

void Testing::agregar_test(Testeable& test) {
    tests_a_correr.push_back(&test);
}

bool Testing::correr_tests() {
    bool todos_los_tests_dan_ok = true;
    for(auto it=tests_a_correr.begin(); it != tests_a_correr.end(); ++it) {
        Testeable* test = *it;
        std::cerr << "----- Ejecutando " << test->obtener_nombre_set() 
                    << "----- " << std::endl;
        test->setup();
        if (!test->ejecutar_set_test())
            todos_los_tests_dan_ok = false;
        test->teardown();
        std::cerr << "------------------------\n" << std::endl;
    }
    return todos_los_tests_dan_ok;
}
/******************************************************************************/

/*********************** Implementación clase Testeable ***********************/
Testeable::Testeable() {
    Testing::obtener_instancia().agregar_test(*this);
}

void Testeable::setup() { }

void Testeable::teardown() { }

bool Testeable::ejecutar_set_test() {
    try {
        correr_tests();
    } catch(const std::exception& e) {
        std::cerr << "ERROR\n" << e.what() << std::endl;
        return false;
    } catch(...) {
        std::cerr << "ERROR\nExcepción desconocida sin controlar." << std::endl;
        return false;
    }
    return true;
}

void Testeable::lanzar_error(int linea, const std::string& test, 
    const std::string& filename, const char *formato, ...) {
    char buffer[1024];
    va_list args;
    va_start (args, formato);
    vsnprintf (buffer, 1024, formato, args);
    va_end (args);
    std::stringstream error_str;
    error_str << "[" << filename << ":" << linea << "] Falló " << test 
              << ": " << buffer;
    throw std::runtime_error(error_str.str());
}
/******************************************************************************/