#include "servidor/servidor.h"

#include <iostream>

namespace servidor {

Servidor::Servidor(const std::string& servicio) 
: aceptador(servicio)
{ }

void Servidor::correr() {
    try {
        while (!terminar) {
            SocketConexion conexion_nueva = aceptador.esperar_conexion();

            clientes.emplace_back(Cliente(std::move(conexion_nueva)));
            lobby.agregar_cliente(clientes.back());

            limpiar_clientes();
            lobby.limpiar_salas();
        }
    } catch (const std::exception& e) {
        if (!terminar)
            std::cerr << "Error en el servidor: " << e.what() << std::endl;
    } catch(...) {
        std::cerr << "Error desconocido en el servidor" << std::endl;
    }
    aceptador.detener();
}

void Servidor::limpiar_clientes() {
    for (auto it = clientes.begin(); it != clientes.end();) {
        Cliente& cliente = *it;
        if (!cliente.esta_conectado()) {
            lobby.notificar_desconexion(cliente);
            cliente.detener_async();
            it = clientes.erase(it);
        } else {
            ++it;
        }
    }
}

void Servidor::detener() {
    terminar = true;
    aceptador.detener();
    lobby.detener_todo();
    for (Cliente& cliente : clientes) {
        cliente.detener_async();
    }
}

Servidor::~Servidor() {
    // Está garantizado que detener no lanza excepciones
    aceptador.detener();
}

} // namespace servidor
