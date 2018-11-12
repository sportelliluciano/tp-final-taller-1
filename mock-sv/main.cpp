#include <chrono>
#include <thread>
#include <list>
#include <mutex>
#include <iostream>
#include <string>
#include "../src/libs/json.hpp"

#include "../src/conexion/conexion.h"
#include "modelo.h"
#include "cola.h"
#include "server_socket_aceptador.h"

static std::mutex lock_cout;

#define COLOR_ROJO      "\x1b[31m"
#define COLOR_VERDE     "\x1b[32m"
#define COLOR_AMARILLO  "\x1b[33m"
#define COLOR_RESET     "\x1b[0m"

using namespace conexion;

class ModeloProtegido {
public:
    ModeloProtegido(Modelo& modelo_) 
        : modelo(modelo_) { }

    void procesar(const nlohmann::json& evento) {
        m.lock();
        modelo.procesar(evento);
        m.unlock();
    }

    void actualizar(int dt) {
        m.lock();
        modelo.actualizar(dt);
        m.unlock();
    }
private:
    std::mutex m;
    Modelo& modelo;
};

class HiloReceptor {
public:
    HiloReceptor(Conexion& conexion_, ModeloProtegido& modelo_, bool& terminar_) 
    : conexion(conexion_), modelo(modelo_), terminar(terminar_)
    { }

    void operator()() {
        try {
            while (conexion.esta_conectada()) {
                nlohmann::json data = conexion.recibir_json();
                lock_cout.lock();
                std::cout << COLOR_VERDE ">> " COLOR_RESET << data.dump() << std::endl;
                lock_cout.unlock();
                modelo.procesar(data);
            }
        } catch(const std::exception& e) {
            std::cout << "HR: conexion perdida (" << e.what() << ")" << std::endl;
        }
        terminar = true;
    }
private:
    Conexion& conexion;
    ModeloProtegido& modelo;
    bool& terminar;
};

class HiloEmisor {
public:
    HiloEmisor(Conexion& conexion_, Cola& cola_, bool& terminar_) 
    : conexion(conexion_), cola(cola_), terminar(terminar_)
    { }

    void operator()() {
        try {
            while (conexion.esta_conectada()) {
                if (!cola.esta_vacia()) {
                    nlohmann::json data = cola.pull();
                    lock_cout.lock();
                    std::cout << COLOR_ROJO "<< " COLOR_RESET << data.dump() << std::endl;
                    lock_cout.unlock();
                    conexion.enviar_json(data);
                }
            }
        } catch(const std::exception& e) {
            std::cout << "HE: conexion perdida (" << e.what() << ")" << std::endl;
        }
        terminar = true;
    }

private:
    Conexion& conexion;
    Cola& cola;
    bool& terminar;
};

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
    Cola cola_salida;
    Modelo modelo_real(cola_salida);
    ModeloProtegido modelo(modelo_real);
    bool terminar = false;

    HiloReceptor receptor(conexion, modelo, terminar);
    HiloEmisor emisor(conexion, cola_salida, terminar);

    // Hilo que recibe eventos
    std::thread tr = std::thread(receptor);

    // Hilo que envia eventos
    std::thread te = std::thread(emisor);

    std::thread in = std::thread([&terminar] () {
        while (std::cin.good()) {
            std::string linea;
            std::getline(std::cin, linea);
            if (linea == "q")
                break;
        }
        terminar = true;
    });

    // Loop principal con ticks de 20ms
    const int tick = 20;
    while (!terminar) {
        std::this_thread::sleep_for(std::chrono::milliseconds(tick));
        modelo.actualizar(tick);
    }
    
    conexion.cerrar(true);
    te.join();
    tr.join();
    in.join();
    std::cout << "Servidor terminado" << std::endl;
    return 0;
}
