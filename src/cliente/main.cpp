#include <iostream>

#include "cliente/cliente_juego.h"

int main(int argc, char *argv[]) {
    // try {
        cliente::ClienteJuego cliente_juego;
        cliente_juego.correr(argc, argv);
    // } catch(const std::exception& e) {
    //     std::cerr << e.what() << std::endl;
    //     return EXIT_FAILURE;
    // } catch(...) {
    //     std::cerr << "Error desconocido" << std::endl;
    //     return EXIT_FAILURE;
    // }
    return EXIT_SUCCESS;
}
