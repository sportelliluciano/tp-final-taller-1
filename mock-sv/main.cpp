#include <chrono>
#include <thread>
#include <list>
#include <mutex>
#include <iostream>
#include <string>

#include "../src/libs/json.hpp"

#include "../src/conexion/conexion.h"
#include "mock_modelo.h"
#include "server_socket_aceptador.h"
#include "partida.h"

using namespace conexion;

/**
 * Pequeño mock del servidor para ir testeando distintas cosas.
 */
int main() {
    SocketAceptador aceptador("9432");
    SocketConexion conexion_nueva;
    if (!aceptador.esperar_conexion(conexion_nueva))
        return 1;
    
    std::cout << "Conexión aceptada" << std::endl;
    

    Conexion conexion(conexion_nueva);

    MockModelo modelo;

    Partida partida(&modelo);
    partida.agregar_jugador(&conexion);

    partida.jugar();

    partida.terminar();
    
    aceptador.detener();
    std::cout << "Servidor terminado" << std::endl;
    return 0;
}
