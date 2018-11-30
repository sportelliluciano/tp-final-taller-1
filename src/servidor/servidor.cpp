#include "servidor/servidor.h"

#include "comun/log.h"

namespace servidor {

Servidor::Servidor(const ProcesadorConfiguracion& configuracion) 
: lobby(configuracion),
  aceptador(configuracion.servicio())
{ }

void Servidor::correr() {
    Log& logger = Log::obtener_instancia();
    try {
        logger.cout("Esperando conexiones...\n");
        while (!terminar) {
            SocketConexion conexion_nueva = aceptador.esperar_conexion();

            clientes.emplace_back(Cliente(std::move(conexion_nueva)));
            lobby.agregar_cliente(clientes.back());

            limpiar_clientes();
            lobby.limpiar_salas();
        }
    } catch (const std::exception& e) {
        if (!terminar)
            log_error("Error en el servidor: %s", e.what());
    } catch(...) {
        log_error("Error desconocido en el servidor.");
    }
    aceptador.detener();
    logger.cout("Se detuvo el servidor\n");
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
