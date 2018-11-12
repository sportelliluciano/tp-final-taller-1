#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../src/libs/json.hpp"
#include "../src/conexion/eventos_cliente.h"
#include "../src/conexion/eventos_servidor.h"
#include "cola.h"

using namespace conexion;

class Modelo {
public:
    Modelo(Cola& cola_salida_) : cola_salida(cola_salida_) { 
    }

    void notificar(const nlohmann::json& evento) {
        cola_salida.push(evento);
    }

    void procesar(const nlohmann::json& evento) {
        evento_servidor_t id = (evento_servidor_t)(evento.at("id").get<int>());

        switch(id) {
            case EVS_INICIAR_CONSTRUCCION:
                if (construcciones_en_cola.count(evento.at("clase")) == 0)
                    construcciones_en_cola[evento.at("clase")] = 1;
                else
                    construcciones_en_cola[evento.at("clase")]++;
                notificar({
                    {"id", EVC_ACTUALIZAR_COLA_CC},
                    {"clase", evento.at("clase")},
                    {"cantidad", construcciones_en_cola[evento.at("clase")]}
                });
                break;
            case EVS_CANCELAR_CONSTRUCCION:
                if (construcciones_en_cola.count(evento.at("clase")) != 0) {
                    construcciones_en_cola[evento.at("clase")]--;
                    if (construcciones_en_cola[evento.at("clase")] <= 0)
                        construcciones_en_cola.erase(evento.at("clase"));
                }
                break;
            case EVS_UBICAR_EDIFICIO: {
                int id_edificio = edificios.size();
                edificios.emplace(
                    id_edificio,
                    Edificio(
                        id_edificio,
                        evento.at("celda").get<std::vector<int>>().at(0),
                        evento.at("celda").get<std::vector<int>>().at(1),
                        0,
                        evento.at("clase")
                    )
                );
                notificar({
                    {"id", EVC_CREAR_EDIFICIO},
                    {"clase", evento.at("clase")},
                    {"posicion", evento.at("celda")},
                    {"id_jugador", 0},
                    {"id_edificio", id_edificio}
                });
                construcciones_esperando_ubicacion.erase(evento.at("clase"));
            } break;
            case EVS_VENDER_EDIFICIO:
                edificios.erase(evento.at("id_edificio"));
                notificar({
                    {"id", EVC_ELIMINAR_EDIFICIO},
                    {"id_edificio", evento.at("id_edificio")}
                });
                break;
            
            case EVS_MOVER_TROPAS: {
                std::vector<int> pos = evento.at("posicion");
                notificar({
                    {"id", EVC_MOVER_TROPA},
                    {"id_tropa", 0},
                    {"camino", {x_tropa, y_tropa, pos.at(0), 
                        pos.at(1)}}
                });
                x_destino = pos.at(0);
                y_destino = pos.at(1);
            } break;
            default:
                break;
        }
    }

    void actualizar_construcciones(int dt) {
        for (auto it=construcciones.begin(); it != construcciones.end();) {
            if (it->second - dt < 0) {
                construcciones_esperando_ubicacion.insert(it->first);
                
                // Construccion terminada
                notificar({
                    {"id", EVC_SINCRONIZAR_CONSTRUCCION},
                    {"clase", it->first},
                    {"tiempo", 0}
                });

                it = construcciones.erase(it);
            } else {
                it->second -= dt;
                ++it;
            }
        }

        for (auto it = construcciones_en_cola.begin(); it != construcciones_en_cola.end();) 
        {
            if ((construcciones.count(it->first) == 0) && 
               (construcciones_esperando_ubicacion.count(it->first) == 0)) 
            {
                construcciones[it->first] = 5000;
                notificar({
                    {"id", EVC_INICIAR_CONSTRUCCION},
                    {"clase", it->first},
                    {"tiempo", 5000}
                });
                it->second--;
                notificar({
                    {"id", EVC_ACTUALIZAR_COLA_CC},
                    {"clase", it->first},
                    {"cantidad", it->second}
                });
            }
            if (it->second == 0) {
                it = construcciones_en_cola.erase(it);
            } else {
                ++it;
            }
        }
    }

    void actualizar_tropas(int dt) {
        float vx = 0, vy = 0;

        if (abs(x_destino - fx_tropa) > 1) {
            vx = (x_destino - fx_tropa) / abs(x_destino - fx_tropa);
            vx *= 0.4 / 15;
        }

        if (abs(y_destino - fy_tropa) > 1) {
            vy = (y_destino - fy_tropa) / abs(y_destino - fy_tropa);
            vy *= 0.4 / 15;
        }

        float dx = vx * dt,
            dy = vy * dt;

        if (abs(dx) > abs(fx_tropa - x_destino))
            fx_tropa = x_destino;
        else
            fx_tropa += dx;
        
        if (abs(dy) > abs(fy_tropa - y_destino))
            fy_tropa = y_destino;
        else
            fy_tropa += dy;
        
        x_tropa = std::floor(fx_tropa);
        y_tropa = std::floor(fy_tropa);
    }

    void actualizar(int dt) {
        if (!tropa_creada) {
            notificar({
                {"id", EVC_CREAR_TROPA},
                {"clase", "a"},
                {"id_tropa", 0},
                {"id_jugador", 0},
                {"posicion", {300, 300}}
            });
            tropa_creada = true;
        }
        actualizar_construcciones(dt);
        actualizar_tropas(dt);
    }

private:
    Cola& cola_salida;

    std::unordered_map<std::string, int> construcciones;
    std::unordered_set<std::string> construcciones_esperando_ubicacion;
    std::unordered_map<std::string, int> construcciones_en_cola;

    struct Edificio {
        int id_edificio;
        int celda_x, celda_y;
        std::string clase;
        int id_jugador;
        Edificio(int id, int x, int y, int jugador, const std::string& clase_)
            : id_edificio(id), celda_x(x), celda_y(y), clase(clase_), 
              id_jugador(jugador)
        { }
    };

    std::unordered_map<int, Edificio> edificios;
    bool tropa_creada = false;
    int x_tropa = 300, y_tropa = 300;
    float fx_tropa = x_tropa, fy_tropa = y_tropa, 
        x_destino = 300, y_destino = 300;
};
