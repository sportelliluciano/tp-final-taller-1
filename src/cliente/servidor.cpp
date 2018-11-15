#include "cliente/servidor.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <stdexcept>
#include <iostream>

#include "libs/json.hpp"

#include "conexion/lock.h"
#include "cliente/eventos/evento.h"
#include "cliente/eventos/factory_evento.h"
#include "cliente/eventos/evento_terminar.h"
#include "conexion/eventos_servidor.h"
#include "cliente/video/log.h"

namespace cliente {

using namespace conexion;

Servidor::Servidor(const std::string& ip_servidor, 
    const std::string& puerto) 
{
    terminar = false;
    conn = new Conexion(ip_servidor, puerto);
}

std::vector<std::string> Servidor::obtener_salas() {
    conn->enviar_json({
        {"tipo", "listar_salas"}
    });

    nlohmann::json data = conn->recibir_json();
    if (data.at("estado") != "OK")
        throw std::runtime_error(data.at("mensaje"));
    
    std::vector<std::string> nombre_salas;
    const std::vector<nlohmann::json>& salas = data.at("salas");

    for (const nlohmann::json& sala : salas) {
        nombre_salas.push_back(sala.at("nombre"));
    }

    return nombre_salas;
}

std::vector<std::string> Servidor::obtener_mapas() {
    conn->enviar_json({
        {"tipo", "listar_mapas"}
    });

    nlohmann::json data = conn->recibir_json();
    if (data.at("estado") != "OK")
        throw std::runtime_error(data.at("mensaje"));
    
    std::vector<std::string> nombre_mapas;
    const std::vector<nlohmann::json>& mapas = data.at("mapas");

    for (const std::string& mapa : mapas) {
        nombre_mapas.push_back(mapa);
    }

    return nombre_mapas;
}

bool Servidor::unirse_a_sala(const std::string& sala) {
    conn->enviar_json({
        {"tipo", "unirse"},
        {"sala", sala}
    });

    nlohmann::json respuesta = conn->recibir_json();
    if (respuesta.at("estado") != "OK")
        return false;
    
    return true;
}

bool Servidor::dejar_sala() {
    conn->enviar_json({
        {"tipo", "dejar_sala"},
    });

    nlohmann::json respuesta = conn->recibir_json();
    if (respuesta.at("estado") != "OK")
        return false;
    
    return true;
}

bool Servidor::crear_sala(const std::string& nombre, const std::string& mapa) {
    conn->enviar_json({
        {"tipo", "crear_sala"},
        {"nombre", nombre},
        {"mapa", mapa}
    });

    nlohmann::json respuesta = conn->recibir_json();
    if (respuesta.at("estado") != "OK")
        return false;
    
    return true;
}

void Servidor::iniciar_juego() {   
    hilo_receptor = std::thread(&Servidor::recibir, this);
}

void Servidor::recibir() {
    try {
        while(conn->esta_conectada()) {
            try {
                Evento *ev = FactoryEvento::crear_desde_json(conn->recibir_json());
                push_evento(ev);
            } catch (const ErrorSocket& e) {
                log_error("Error socket: %s", e.what());
                break;
            } catch (const std::exception& e) {
                log_error("Datos inválidos: %s", e.what());
            }
        }
        /*while (std::cin.good()) {
            std::string data;
            std::getline(std::cin, data);
            if (data == "")
                continue;
            try {
                Evento* ev = 
                    FactoryEvento::crear_desde_json(
                        nlohmann::json::parse(data)
                    );
                push_evento(ev);
            } catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
            } catch(...) {
                std::cout << "Error al parsear JSON" << std::endl;
            }
        }*/
        push_evento(new EventoTerminar());
    } catch (const std::exception& e) {
        std::cout << "Error en hilo servidor: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "El hilo died" << std::endl;
    }
}

bool Servidor::hay_eventos() const {
    // Sólo se accede al contenedor así que no hay race conditions acá.
    // http://www.cplusplus.com/reference/list/list/empty/
    return !cola_eventos.empty();
}

Evento* Servidor::pop_evento() {
    Lock l(cola_eventos_mutex);
    if (cola_eventos.empty())
        throw std::runtime_error("La cola está vacía");
    
    Evento* evento = cola_eventos.back();
    cola_eventos.pop_back();
    
    return evento;
}

void Servidor::push_evento(Evento* evento) {
    Lock l(cola_eventos_mutex);
    cola_eventos.push_front(evento);
}

void Servidor::enviar_evento(const nlohmann::json& evento) {
    std::cout << "<< " << evento.dump() << std::endl;
    conn->enviar_json(evento);
}

void Servidor::iniciar_construccion(const std::string& clase) {
    enviar_evento({
        {"id", EVS_INICIAR_CONSTRUCCION},
        {"clase", clase}
    });
}

void Servidor::cancelar_construccion(const std::string& clase) {
    enviar_evento({
        {"id", EVS_CANCELAR_CONSTRUCCION},
        {"clase", clase}
    });
}

void Servidor::vender_edificio(int id) {
    enviar_evento({
        {"id", EVS_VENDER_EDIFICIO},
        {"id_edificio", id}
    });
}

void Servidor::ubicar_edificio(const std::string& clase, int celda_x, 
    int celda_y) 
{
    enviar_evento({
        {"id", EVS_UBICAR_EDIFICIO},
        {"clase", clase},
        {"celda", {celda_x, celda_y}}
    });
}

void Servidor::iniciar_entrenamiento(const std::string& clase) {
    enviar_evento({
        {"id", EVS_INICIAR_ENTRENAMIENTO},
        {"clase", clase}
    });
}

void Servidor::cancelar_entrenamiento(const std::string& clase) {
    enviar_evento({
        {"id", EVS_CANCELAR_ENTRENAMIENTO},
        {"clase", clase}
    });
}

void Servidor::mover_tropas(const std::vector<int>& ids, int x_px, int y_px) {
    enviar_evento({
        {"id", EVS_MOVER_TROPAS},
        {"ids_tropa", ids},
        {"posicion", {x_px, y_px}}
    });
}

void Servidor::atacar_tropa(const std::vector<int>& ids, int id_atacado) {
    enviar_evento({
        {"id", EVS_ATACAR_TROPA},
        {"ids_tropa", ids},
        {"id_atacado", id_atacado}
    });
}

void Servidor::indicar_especia_cosechadora(const std::vector<int>& ids, 
    int celda_x, int celda_y)
{
    enviar_evento({
        {"id", EVS_COSECHADORA_INDICAR_ESPECIA},
        {"ids_tropa", ids},
        {"celda", {celda_x, celda_y}}
    });
}

void Servidor::detener() {
    conn->cerrar(true);
    delete conn;
    if (hilo_receptor.joinable())
        hilo_receptor.join();
}

Servidor::~Servidor() {
    for (auto it=cola_eventos.begin(); it != cola_eventos.end(); ++it)
        delete *it;
    
    // delete conn;
}

} // namespace cliente
