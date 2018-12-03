#include "servidor/lobby.h"

#include <unordered_set>

#include "libs/json.hpp"

#include "comun/lock.h"
#include "servidor/cliente.h"
#include "servidor/procesador_configuracion.h"
#include "modelo/juego.h"

namespace servidor {

Lobby::Lobby(const ProcesadorConfiguracion& configuracion) 
: administrador_mapas(configuracion.obtener_mapas()),
  data_edificios(configuracion.obtener_edificios()),
  data_ejercitos(configuracion.obtener_ejercitos())
{ }

void Lobby::agregar_cliente(Cliente& cliente) {
    if (lobby_cerrado) {
        throw std::runtime_error("El lobby está cerrado");
    }

    cliente.al_recibir_datos(
        [this, &cliente] (const nlohmann::json& dato) {
            procesar_evento(cliente, dato);
        }
    );

    cliente.iniciar_async();
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

    bool enviar_ok = false;

    const std::string& tipo = evento.at("tipo");

    if (tipo == "listar_salas") {
        listar_salas(cliente);
    } else if (tipo == "listar_mapas") {
        listar_mapas(cliente);
    } else if (tipo == "unirse") {
        enviar_ok = unirse(cliente, evento.at("sala"));
    } else if (tipo == "dejar_sala") {
        enviar_ok = dejar_sala(cliente);
    } else if (tipo == "crear_sala") {
        enviar_ok = crear_sala(cliente, evento.at("nombre"), evento.at("mapa"));
    } else if (tipo == "iniciar_juego") {
        enviar_ok = iniciar_juego(cliente);
    } else if (tipo == "setear_casa") {
        enviar_ok = set_casa(cliente, evento.at("casa"));
    } else if (tipo == "setear_nombre") {
        enviar_ok = set_nombre(cliente, evento.at("nombre"));
    } else if (tipo == "listar_jugadores") {
        listar_jugadores(cliente);
    } else {
        enviar_error(cliente, "Acción desconocida");
    }

    if (enviar_ok)
        cliente.enviar({{"estado", "OK"}});
}

void Lobby::listar_salas(Cliente& cliente) {
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
}

void Lobby::listar_mapas(Cliente& cliente) {
    cliente.enviar({
        {"estado", "OK"},
        {"mapas", administrador_mapas.obtener_nombres()}
    });
}

bool Lobby::unirse(Cliente& cliente, const std::string& nombre_sala) {
    if (salas_clientes.count(&cliente) != 0) {
        enviar_error(cliente,
            "Tenés que salir de la sala actual para unirte a otra");
        return false;
    }

    auto it = salas.find(nombre_sala);

    if (it == salas.end()) {
        enviar_error(cliente, "Sala inexistente");
        return false;
    }

    if (it->second.puede_unirse()) {
        it->second.agregar_cliente(cliente);
    } else {
        enviar_error(cliente, "No se puede unir a esta sala");
        return false;
    }
    
    salas_clientes[&cliente] = &it->second;
    return true;
}

bool Lobby::dejar_sala(Cliente& cliente) {
    if (salas_clientes.count(&cliente) == 0) {
        enviar_error(cliente, "No estás en una sala");
        return false;
    }
    salas_clientes.at(&cliente)->eliminar_cliente(cliente);
    salas_clientes.erase(&cliente);
    return true;
}

bool Lobby::crear_sala(Cliente& cliente, const std::string& nombre, 
    const std::string& mapa)
{
    if (salas.count(nombre) != 0) {
        enviar_error(cliente, "Ya hay una sala con ese nombre");
        return false;
    }

    if (!administrador_mapas.existe_mapa(mapa)) {
        enviar_error(cliente, "No existe el mapa indicado");
        return false;
    }
    
    modelo::Juego *nuevo_juego = new modelo::Juego();
    nuevo_juego->inicializar(
        administrador_mapas.obtener_mapa(mapa),
        data_edificios,
        data_ejercitos);
    
    salas.emplace(nombre, Sala(nombre, 
        administrador_mapas.obtener_cantidad_jugadores(mapa), nuevo_juego));
    
    auto it = salas.find(nombre);
    it->second.agregar_cliente(cliente);
    salas_clientes[&cliente] = &it->second;
    return true;
}

bool Lobby::iniciar_juego(Cliente& cliente) {
    if (salas_clientes.count(&cliente) == 0) {
        enviar_error(cliente, "Debes estar en una sala para iniciar el juego");
        return false;
    }

    salas_clientes[&cliente]->iniciar_partida(cliente);
    return true;
}

bool Lobby::set_casa(Cliente& cliente, const std::string& casa) {
    if (salas_clientes.count(&cliente) == 0) {
        enviar_error(cliente, "Debes estar en una sala para iniciar el juego");
        return false;
    }

    return salas_clientes[&cliente]->set_casa_cliente(cliente, casa);
}

bool Lobby::set_nombre(Cliente& cliente, const std::string& nombre) {
    cliente.set_nombre(nombre);
    return true;
}

void Lobby::listar_jugadores(Cliente& cliente) {
    if (salas_clientes.count(&cliente) == 0) {
        enviar_error(cliente, "Debes estar en la sala para ver sus jugadores");
        return;
    }

    salas_clientes[&cliente]->listar_jugadores(cliente);
}


void Lobby::limpiar_salas() {
    for (auto it = salas.begin(); it != salas.end(); ) {
        Sala& sala = it->second;

        if (sala.cantidad_jugadores_conectados() == 0) {
            sala.terminar_partida();
            it = salas.erase(it);
        } else {
            ++it;
        }
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
