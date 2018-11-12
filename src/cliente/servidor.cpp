#include "cliente/servidor.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <stdexcept>
#include <iostream>

#include "libs/json.hpp"

#include "cliente/video/lock.h"
#include "cliente/eventos/evento.h"
#include "cliente/eventos/factory_evento.h"
#include "cliente/eventos/evento_terminar.h"
#include "conexion/eventos_servidor.h"
#include "cliente/video/log.h"

namespace cliente {

using namespace conexion;

Servidor::Servidor() { 
}

void Servidor::iniciar() {
    terminar = false;
    conn = new Conexion("localhost", "9432");
    hilo_receptor = std::thread(&Servidor::recibir, this);
}

void Servidor::recibir() {
    try {
        while(conn->esta_conectada()) {
            try {
                Evento *ev = FactoryEvento::crear_desde_json(conn->recibir_json());
                push_evento(ev);
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
    conn->cerrar();
    delete conn;
    hilo_receptor.join();
}

Servidor::~Servidor() {
    for (auto it=cola_eventos.begin(); it != cola_eventos.end(); ++it)
        delete *it;
    
    // delete conn;
}

} // namespace cliente
