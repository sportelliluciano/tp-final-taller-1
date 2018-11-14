#include "servidor/lobby.h"

#include <unordered_set>

#include "conexion/lock.h"
#include "servidor/cliente.h"

namespace servidor {

void Lobby::agregar_cliente(Cliente& cliente) {
    if (lobby_cerrado) {
        throw std::runtime_error("El lobby está cerrado");
    }

    cliente.al_recibir_datos(
        [this, &cliente] (const nlohmann::json& dato) {
            procesar_evento(cliente, dato);
        }
    );

    clientes.insert(&cliente);
}

void Lobby::notificar_desconexion(Cliente& cliente) {
    Lock l(m_lobby);
    
    cliente.al_recibir_datos(nullptr);
    auto it = salas_clientes.find(&cliente);

    if (it != salas_clientes.end()) {
        it->second->notificar_desconexion(cliente);
        salas_clientes.erase(it);
    }

    clientes.erase(&cliente);
}

static void enviar_error(Cliente& cliente, const std::string& mensaje) {
    cliente.enviar({
        {"estado", "ERROR"},
        {"mensaje", mensaje}
    });
}

void Lobby::procesar_evento(Cliente& cliente, const nlohmann::json& evento) {
    Lock l(m_lobby);

    if (lobby_cerrado)
        return;

    const std::string& tipo = evento.at("tipo");

    if (tipo == "listar_salas") {
        std::vector<nlohmann::json> data_salas;
        for (auto& par : salas) {
            data_salas.push_back({
                {"nombre", par.first},
                {"capacidad", par.second.obtener_capacidad()}
            });
        }
        cliente.enviar({
            {"estado", "OK"},
            {"salas", data_salas}
        });
    } else if (tipo == "listar_mapas") {
        cliente.enviar({
            {"estado", "OK"},
            {"mapas", {"mapa-2-jugadores", "mapa-4-jugadores"}}
        });
    } else if (tipo == "unirse") {
        if (salas_clientes.count(&cliente) != 0) {
            enviar_error(cliente,
                "Tenés que salir de la sala actual para unirte a otra");
            return;
        }

        auto it = salas.find(evento.at("sala"));

        if (it == salas.end()) {
            enviar_error(cliente, "Sala inexistente");
            return;
        }

        it->second.agregar_cliente(cliente);
        salas_clientes[&cliente] = &it->second;
        cliente.enviar({
            {"estado", "OK"}
        });
    } else if (tipo == "dejar_sala") {
        if (salas_clientes.count(&cliente) == 0) {
            enviar_error(cliente, "No estás en una sala");
            return;
        }
        salas_clientes.at(&cliente)->eliminar_cliente(cliente);
        cliente.enviar({
            {"estado", "OK"}
        });
    } else if (tipo == "crear_sala") {
        const std::string& nombre = evento.at("nombre");
        const std::string& mapa = evento.at("mapa");
        if (salas.count(nombre) != 0) {
            enviar_error(cliente, "Ya hay una sala con ese nombre");
            return;
        } 
        
        if (mapa == "mapa-2-jugadores") {
            salas.emplace(nombre, Sala(2));
        } else if (mapa == "mapa-4-jugadores") {
            salas.emplace(nombre, Sala(4));
        } else {
            enviar_error(cliente, "El mapa no existe");
            return;
        }

        auto it = salas.find(nombre);

        it->second.agregar_cliente(cliente);
        salas_clientes[&cliente] = &it->second;
        
        cliente.enviar({
            {"estado", "OK"}
        });
    } else if (tipo == "iniciar_juego") {
        if (salas_clientes.count(&cliente) == 0) {
            enviar_error(cliente, 
                "Debes estar en una sala para iniciar el juego");
            return;
        }

        salas_clientes[&cliente]->iniciar_partida(cliente);
    } else {
        enviar_error(cliente, "Acción desconocida");
    }

}

void Lobby::detener_todo() {
    Lock l(m_lobby);
    
    lobby_cerrado = true;
    
    for (Cliente* cliente : clientes) {
        cliente->al_recibir_datos(nullptr);
    }

    for (auto it = salas_clientes.begin(); it != salas_clientes.end(); ++it) {
        it->second->terminar_partida();
    }
}

} // namespace servidor
