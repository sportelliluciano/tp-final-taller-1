#include "hilo.h"

#include <exception>
#include <string>
#include <thread>

void Hilo::iniciar() {
    if (fue_iniciado)
        throw std::runtime_error("Se intentó iniciar un hilo ya iniciado");
    
    fue_iniciado = true;
    hilo = std::thread(&Hilo::wrapper_correr, this);
}

void Hilo::detener() {
    if (!fue_iniciado)
        throw std::runtime_error("Se intentó detener un hilo no iniciado");
    solicitar_detencion();
    join();
}

bool Hilo::esta_corriendo() const noexcept {
    return fue_iniciado && !hilo_termino;
}

void Hilo::join() {
    hilo.join();
}

const std::string& Hilo::obtener_error() const {
    if (!hilo_termino)
        throw std::runtime_error("El hilo sigue corriendo");
    
    if (!hubo_excepcion)
        throw std::runtime_error("No se produjo ninguna excepción");
    
    return msj_error;
}

bool Hilo::termino_correctamente() const {
    if (!hilo_termino)
        throw std::runtime_error("El hilo sigue corriendo");

    return !hubo_excepcion;
}

Hilo::~Hilo() noexcept {
    if (esta_corriendo()) {
        try {
            hilo.join();
        } catch(...) {
            // Prevenir excepciones en el destructor.
            // Si se produjera alguna excepción en este punto no
            //  hay nada que se pueda hacer.
        }
    }
}

void Hilo::wrapper_correr() {
    try {
        correr();
    } catch(std::runtime_error& e) {
        msj_error = e.what();
        hubo_excepcion = true;
    } catch(...) {
        msj_error = "Excepción desconocida";
        hubo_excepcion = true;
    }

    hilo_termino = true;
}
