#ifndef _HILO_RECEPTOR_H_
#define _HILO_RECEPTOR_H_

#include <iostream>
#include <mutex>

#include "../src/libs/json.hpp"

#include "../src/conexion/conexion.h"
#include "../src/conexion/eventos_servidor.h"
#include "i_modelo.h"
#include "i_jugador.h"

class HiloReceptor {
public:
    HiloReceptor(conexion::Conexion& conexion_, IModelo* modelo_, 
        std::mutex& lock_modelo_, IJugador* jugador_) 
    : conexion(conexion_), modelo(modelo_), lock_modelo(lock_modelo_), 
      jugador(jugador_)
    { }

    void operator()() {
        using namespace conexion;
        try {
            while (conexion.esta_conectada()) {
                nlohmann::json evento = conexion.recibir_json();
                std::cout << COLOR_VERDE ">> " COLOR_RESET << evento.dump() << std::endl;
                evento_servidor_t id = evento.at("id").get<evento_servidor_t>();
                lock_modelo.lock();
                switch(id) {
                    case EVS_INICIAR_CONSTRUCCION:
                        modelo->iniciar_construccion_edificio(
                            jugador, 
                            evento.at("clase")
                        );
                        break;
                    
                    case EVS_CANCELAR_CONSTRUCCION:
                        modelo->cancelar_construccion_edificio(
                            jugador, 
                            evento.at("clase")
                        );
                        break;
                    
                    case EVS_UBICAR_EDIFICIO: 
                        modelo->ubicar_edificio(
                            jugador, 
                            evento.at("celda").get<std::vector<int>>().at(0),
                            evento.at("celda").get<std::vector<int>>().at(1),
                            evento.at("clase")
                        );
                        break;
                    
                    case EVS_VENDER_EDIFICIO:
                        modelo->vender_edificio(
                            jugador,
                            evento.at("id_edificio")
                        );
                        break;

                    case EVS_INICIAR_ENTRENAMIENTO:
                        modelo->iniciar_entrenamiento_tropa(
                            jugador, 
                            evento.at("clase")
                        );
                        break;
                    
                    case EVS_CANCELAR_ENTRENAMIENTO:
                        modelo->cancelar_entrenamiento_tropa(
                            jugador, 
                            evento.at("clase")
                        );
                        break;
                    
                    case EVS_MOVER_TROPAS: 
                        modelo->mover_tropas(
                            jugador,
                            evento.at("ids_tropa"),
                            evento.at("posicion").get<std::vector<int>>().at(0),
                            evento.at("posicion").get<std::vector<int>>().at(1)
                        );
                        break;

                    case EVS_ATACAR_TROPA:
                        modelo->atacar_tropa(
                            jugador,
                            evento.at("ids_tropa"),
                            evento.at("id_atacado")
                        );
                        break;
                    
                    case EVS_COSECHADORA_INDICAR_ESPECIA:
                        modelo->indicar_especia_cosechadora(
                            jugador,
                            evento.at("ids_tropa"),
                            evento.at("celda").get<std::vector<int>>().at(0),
                            evento.at("celda").get<std::vector<int>>().at(1)
                        );
                        break;
                    
                    default:
                        std::cout << "Evento desconocido" << std::endl;
                        break;
                }
                lock_modelo.unlock();                
            }
        } catch(const std::exception& e) {
            std::cout << "HR: conexion perdida (" << e.what() << ")" << std::endl;
        }
        terminar = true;
        modelo->jugador_desconectado(jugador);
    }

    bool termino() const {
        return terminar;
    }
private:
    conexion::Conexion& conexion;
    IModelo* modelo;
    IJugador* jugador;
    std::mutex& lock_modelo;

    bool terminar = false;
    int id_jugador;
};

#endif // _HILO_RECEPTOR_H_
