#include <iostream>
#include <exception>

#include <SDL2/SDL.h>

#include "cliente/cliente_juego.h"
#include "cliente/servidor.h"

int main(int argc, char *argv[]) {
    SDL_Init(0);
    cliente::Servidor servidor(argc, argv);
    cliente::ClienteJuego cliente_juego(servidor, argc, argv);
    int retcode = EXIT_SUCCESS;
    
    try {
        servidor.iniciar();
        retcode = cliente_juego.ejecutar();
    } catch (const std::exception& e) {
        std::cerr << "Se produjo un error: " << e.what() << std::endl;
        retcode = EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Se produjo un error desconocido" << std::endl;
        retcode = EXIT_FAILURE;
    }
    
    try {
        servidor.detener();
    } catch(...) {
        // Preveer excepciones en caso de que no se pueda 
        //  hacerle join al hilo.
    }
    SDL_Quit();
    return retcode;
}
