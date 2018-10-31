#include "cliente/servidor.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <stdexcept>

#include "cliente/video/lock.h"
#include "conexion/eventos/evento_crear_edificio.h"

namespace cliente {

Servidor::Servidor(int argc, char *argv[]) { 
    /*if (argc == 3) {
        conn = new conexion::Conexion(argv[1], argv[2]);
    } else {
        throw std::runtime_error("Indique a donde conectarse");
    }*/
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
    terminar = true;
    hilo_receptor.join();
}

Servidor::~Servidor() {
    for (auto it=cola_eventos.begin(); it != cola_eventos.end(); ++it)
        delete *it;
    
    // delete conn;
}

} // namespace cliente
