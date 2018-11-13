#ifndef _PARTIDA_H_
#define _PARTIDA_H_

#include <chrono>
#include <unordered_map>
#include <stdexcept>
#include <mutex>
#include <thread>

#include "i_modelo.h"
#include "../src/conexion/conexion.h"
#include "conexion_jugador.h"

class Partida {
public:
    Partida(IModelo* modelo_) {
        modelo = modelo_;
    }

    void agregar_jugador(conexion::Conexion* jugador) {
        if (modelo->partida_iniciada())
            throw std::runtime_error("La partida ya fue iniciada");

        ultimo_id++;
        jugadores[ultimo_id] = new ConexionJugador(modelo, lock_modelo, 
            jugador, ultimo_id);
        modelo->crear_jugador(jugadores[ultimo_id]);
    }

    void jugar() {
        modelo->iniciar_partida();

        bool terminar = false;

        std::thread in = std::thread([&terminar] () {
            while (std::cin.good()) {
                std::string linea;
                std::getline(std::cin, linea);
                if (linea == "q")
                    break;
            }
            terminar = true;
        });
        
        const int tick = 20;
        while (!terminar && !modelo->juego_terminado()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(tick));
            lock_modelo.lock();
            modelo->actualizar(tick);
            lock_modelo.unlock();
        }
    }

    void terminar() {
        for (auto it = jugadores.begin(); it != jugadores.end();) {
            it->second->cerrar_conexion();
            it = jugadores.erase(it);
        }
    }

private:
    IModelo *modelo = nullptr;
    std::unordered_map<int, ConexionJugador*> jugadores;
    int ultimo_id = 0;
    std::mutex lock_modelo;
};

#endif // _PARTIDA_H_