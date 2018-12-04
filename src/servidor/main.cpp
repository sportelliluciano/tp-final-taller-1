#include <iostream>
#include <string>

#include "comun/log.h"
#include "servidor/config.h"
#include "servidor/error_configuracion.h"
#include "servidor/servidor.h"
#include "servidor/procesador_configuracion.h"

#define CENTINELA_DETENER "q"

int main(int argc, char *argv[]) {
    using namespace servidor;
    Log& logger = Log::obtener_instancia();
    try {
        ProcesadorConfiguracion configuracion(argc, argv);
        if (configuracion.imprimir_ayuda()) {
            logger.cout(
                "Uso: ./dune-remake-servidor [-p --puerto | -h --help | "
                "-c --config]\n"
                "\t-p: Indica el puerto en el cual el servidor escuchará.\n"
                "\tEsto ignorará otras configuraciones de puerto\n"
                "\t-h: Muestra esta ayuda.\n"
                "\t-c: Carga el archivo de configuración indicado.\n\tSi este "
                "parámetro no se especifica, se cargará la configuración desde "
                "%s.\n", RUTA_CONFIG_JSON);
            return EXIT_SUCCESS;
        }
        configuracion.mostrar_configuracion();
        Servidor servidor(configuracion);
        std::thread hilo_servidor = std::thread(&Servidor::correr, &servidor);
        logger.cout("Ingrese '%s' para salir\n", 
            CENTINELA_DETENER);
        while (std::cin.good()) {
            std::string linea;
            std::getline(std::cin, linea);
            if (linea == CENTINELA_DETENER)
                break;
        }
        logger.cout("Deteniendo el servidor... Esto puede llevar un tiempo.\n");
        servidor.detener();
        hilo_servidor.join();
    } catch(const ErrorConfiguracion& e) {
        logger.cerr("%s\n", e.what());
        return EXIT_FAILURE;
    } catch(const std::exception& e) {
        logger.cerr("Se produjo un error: %s\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
