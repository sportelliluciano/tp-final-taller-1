#include "cliente/servidor.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <stdexcept>
#include <iostream>

#include "libs/json.hpp"

#include "cliente/video/lock.h"
#include "conexion/evento.h"
#include "conexion/eventos/evento_juego_terminado.h"
#include "conexion/factory_evento.h"

namespace cliente {

Servidor::Servidor() { 
}

void Servidor::iniciar() {
    terminar = false;
    hilo_receptor = std::thread(&Servidor::recibir, this);
}

void Servidor::recibir() {
    try {
        /*while(conn->esta_conectada()) {
            conexion::Evento *ev = conn->recibir_evento();
            push_evento(ev);
        }*/
        while (std::cin.good()) {
            std::string data;
            std::getline(std::cin, data);
            if (data == "")
                continue;
            try {
                conexion::Evento* ev = 
                    conexion::FactoryEvento::crear_desde_json(
                        nlohmann::json::parse(data)
                    );
                push_evento(ev);
            } catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
            } catch(...) {
                std::cout << "Error al parsear JSON" << std::endl;
            }
        }
        push_evento(new conexion::EventoJuegoTerminado());
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

conexion::Evento* Servidor::pop_evento() {
    Lock l(cola_eventos_mutex);
    if (cola_eventos.empty())
        throw std::runtime_error("La cola está vacía");
    
    conexion::Evento* evento = cola_eventos.back();
    cola_eventos.pop_back();
    
    return evento;
}

void Servidor::push_evento(conexion::Evento* evento) {
    Lock l(cola_eventos_mutex);
    cola_eventos.push_front(evento);
}

void Servidor::detener() {
    hilo_receptor.join();
}

Servidor::~Servidor() {
    for (auto it=cola_eventos.begin(); it != cola_eventos.end(); ++it)
        delete *it;
    
    // delete conn;
}

} // namespace cliente
