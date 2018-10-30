#include <iostream>
#include <exception>

#include "cliente/cliente_juego.h"

int main(int argc, char *argv[]) {
    try {
        cliente::ClienteJuego cliente_juego(argc, argv);
        return cliente_juego.ejecutar();
    } catch (const std::exception& e) {
        std::cerr << "Se produjo un error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Se produjo un error desconocido" << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
