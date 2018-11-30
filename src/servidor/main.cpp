#include <iostream>
#include <string>

#include "servidor/config.h"
#include "servidor/servidor.h"

int main(int argc, char *argv[]) {
    using namespace servidor;
    
    std::string servicio = "9432";
    if (argc == 2)
        servicio = argv[1];
    
    if (argc > 2) {
        std::cerr << "Uso: ./servidor [puerto]" << std::endl;
        return EXIT_FAILURE;
    }
    
    try {
        Servidor servidor(servicio);
        std::thread hilo_servidor = std::thread(&Servidor::correr, &servidor);
        while (std::cin.good()) {
            std::string linea;
            std::getline(std::cin, linea);
            if (linea == "q")
                break;
        }
        servidor.detener();
        hilo_servidor.join();
    } catch(const std::exception& e) {
        std::cerr << "Se produjo un error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
