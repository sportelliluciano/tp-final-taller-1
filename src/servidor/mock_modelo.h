#ifndef _MOCK_MODELO_H_
#define _MOCK_MODELO_H_

#include <fstream>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "libs/json.hpp"

#include "conexion/i_modelo.h"

#define DINERO_INICIAL 1000
#define DINERO_MAXIMO_BASE 1000

#define ENERGIA_INICIAL 1000
#define ENERGIA_MAXIMA_BASE 1000

namespace servidor {

using namespace conexion;

class MockModelo : public IModelo {
public:
    MockModelo() { }

    void inicializar(const nlohmann::json& mapa_, 
        const nlohmann::json& edificios_, const nlohmann::json& ejercito_) 
    {
        mapa = mapa_;
        infraestructura = edificios_;
        ejercito = ejercito_;
    }

    void crear_jugador(IJugador* jugador) {
        std::vector<std::vector<int>> pos_jugadores = mapa.at("jugadores");
        Edificio& centro = crear_edificio(
            pos_jugadores.at(jugadores.size()).at(0),
            pos_jugadores.at(jugadores.size()).at(1),
            jugador->obtener_id(),
            "centro_construccion"
        );
        jugadores.emplace(jugador, Jugador(centro));
        jugadores.at(jugador).edificios.insert({centro.id_edificio, &centro});
    }

    void iniciar_partida() {
        // Crear centros de construcción y enviar parámetros iniciales
        // a los clientes
        broadcast([this](IJugador* j) {
            j->actualizar_dinero(DINERO_INICIAL, DINERO_MAXIMO_BASE);
            j->actualizar_energia(ENERGIA_INICIAL, ENERGIA_MAXIMA_BASE);
            for (auto it=jugadores.begin(); it != jugadores.end(); ++it) {
                Edificio& centro = jugadores.at(it->first).centro_construccion;
                j->agregar_edificio(
                    centro.id_edificio,
                    centro.clase, 
                    centro.celda_x, centro.celda_y, 
                    centro.id_propietario,
                    3000
                );
            }
        });
    }

    void actualizar(int dt) {
        for (auto& it : jugadores) {
            actualizar_construcciones(it.first, dt);
            actualizar_entrenamientos(it.first, dt);
            actualizar_tropas(it.first, dt);
        }
    }

    bool partida_terminada() const {
        return terminado;
    }

    void jugador_desconectado(IJugador* jugador) {
        jugadores.erase(jugador);
    }

    void jugador_listo(IJugador* jugador) {
        jugadores.at(jugador).listo = true;
    }

    void iniciar_construccion_edificio(IJugador* jugador_act,
        const std::string& clase)
    {
        auto& construcciones_en_cola = 
            jugadores.at(jugador_act).construcciones_en_cola;
        
        if (construcciones_en_cola.count(clase) == 0)
            construcciones_en_cola.insert({clase, 1});
        else
            construcciones_en_cola.at(clase)++;
        
        jugador_act->actualizar_cola_cc(clase, construcciones_en_cola.at(clase));
    }
    
    void cancelar_construccion_edificio(IJugador* jugador_act, 
        const std::string& clase)
    {
        auto& construcciones_en_cola = 
            jugadores.at(jugador_act).construcciones_en_cola;

        if (construcciones_en_cola.count(clase) != 0) {
            construcciones_en_cola.at(clase)--;
            jugador_act->actualizar_cola_cc(clase, 
                construcciones_en_cola.at(clase));
            if (construcciones_en_cola.at(clase) <= 0)
                construcciones_en_cola.erase(clase);
        }
    }
    
    void ubicar_edificio(IJugador* jugador_act, int celda_x, int celda_y,
        const std::string& clase)
    {
        auto& edificios = jugadores.at(jugador_act).edificios;
        auto& construcciones_esperando_ubicacion 
            = jugadores.at(jugador_act).construcciones_esperando_ubicacion;

        Edificio& nuevo = crear_edificio(celda_x, celda_y, jugador_act->obtener_id(), clase);
        jugador_act->crear_edificio(
            nuevo.id_edificio,
            nuevo.clase, 
            nuevo.celda_x, nuevo.celda_y, 
            nuevo.id_propietario
        );
        edificios.insert({nuevo.id_edificio, &nuevo});
        construcciones_esperando_ubicacion.erase(clase);

        broadcast([this, jugador_act, &clase, celda_x, celda_y] (IJugador* j) {
            if (j != jugador_act) {
                j->agregar_edificio(
                    id_edificio,
                    clase, 
                    celda_x, celda_y, 
                    jugador_act->obtener_id(),
                    100
                );
            }
        });

        id_edificio++;
    }

    void vender_edificio(IJugador* jugador_act, int id) {
        auto& edificios = jugadores.at(jugador_act).edificios;
        edificios.erase(id);
        broadcast([this, id] (IJugador* j) {
            j->eliminar_edificio(id);
        });
    }

    void iniciar_entrenamiento_tropa(IJugador* jugador_act,
        const std::string& clase) 
    {
        auto& entrenamientos_en_cola = 
            jugadores.at(jugador_act).entrenamientos_en_cola;
        
        if (entrenamientos_en_cola.count(clase) == 0)
            entrenamientos_en_cola.insert({clase, 1});
        else
            entrenamientos_en_cola.at(clase)++;
        
        jugador_act->actualizar_cola_ee(clase, entrenamientos_en_cola.at(clase));
    }

    void cancelar_entrenamiento_tropa(IJugador* jugador_act,
        const std::string& clase)
    {
        auto& entrenamientos_en_cola = 
            jugadores.at(jugador_act).entrenamientos_en_cola;

        if (entrenamientos_en_cola.count(clase) != 0) {
            entrenamientos_en_cola.at(clase)--;
            jugador_act->actualizar_cola_ee(clase, entrenamientos_en_cola.at(clase));
            if (entrenamientos_en_cola.at(clase) <= 0)
                entrenamientos_en_cola.erase(clase);
        }
    }

    void mover_tropas(IJugador* jugador_act, const std::unordered_set<int>& ids,
        int x_px, int y_px) override
    {
        unsigned int cant = sqrt(ids.size());
        unsigned int n = 0;
        for (int id : ids) {
            Tropa* t = jugadores.at(jugador_act).tropas.at(id);
            t->x_destino = x_px + n * 16;
            t->y_destino = y_px;
            n++;
            if (n == cant) {
                y_px += 16;
                n = 0;
            }
            

            broadcast([t] (IJugador* j) {
                j->mover_tropa(t->id, 
                    { {t->x_actual, t->y_actual}, {t->x_destino, t->y_destino}});
            });
        }
    }

    void atacar_tropa(IJugador*, 
        const std::unordered_set<int>&, int) 
    {
    }

    void indicar_especia_cosechadora(IJugador*,
        const std::unordered_set<int>&, int, int)
    {
    }

    virtual ~MockModelo() noexcept { }

private:

    void actualizar_construcciones(IJugador* jugador, int dt) {
        auto& construcciones = jugadores.at(jugador).construcciones;
        auto& construcciones_en_cola = jugadores.at(jugador).construcciones_en_cola;
        auto& construcciones_esperando_ubicacion = 
            jugadores.at(jugador).construcciones_esperando_ubicacion;

        for (auto it=construcciones.begin(); it != construcciones.end();) {
            if (it->second - dt < 0) {
                construcciones_esperando_ubicacion.insert(it->first);
                
                // Construccion terminada
                jugador->sincronizar_construccion(it->first, 0);
                
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
                jugador->iniciar_construccion(it->first, 5000);
                it->second--;
                jugador->actualizar_cola_cc(it->first, it->second);
            }
            if (it->second == 0) {
                it = construcciones_en_cola.erase(it);
            } else {
                ++it;
            }
        }
    }

    void actualizar_entrenamientos(IJugador *jugador, int dt) {
        auto& entrenamientos = jugadores.at(jugador).entrenamientos;
        auto& entrenamientos_en_cola = jugadores.at(jugador).entrenamientos_en_cola;
        auto& tropas = jugadores.at(jugador).tropas;

        for (auto it=entrenamientos.begin(); it != entrenamientos.end();) {
            if (it->second - dt < 0) {
                // Entrenamiento terminado
                jugador->sincronizar_entrenamiento(it->first, 0);
                broadcast([this, jugador, it] (IJugador* j) {
                    j->crear_tropa(id_tropa, it->first, 
                        300, 300, 100, jugador->obtener_id());
                });
                tropas_global.insert({id_tropa, Tropa(id_tropa, 300, 300, 100, 
                    jugador->obtener_id())});
                tropas.insert({id_tropa, &tropas_global.at(id_tropa)});
                id_tropa++;
                it = entrenamientos.erase(it);
            } else {
                it->second -= dt;
                ++it;
            }
        }

        for (auto it = entrenamientos_en_cola.begin(); it != entrenamientos_en_cola.end();) 
        {
            if (entrenamientos.count(it->first) == 0) {
                entrenamientos[it->first] = 60000;
                jugador->iniciar_entrenamiento(it->first, 60000);
                it->second--;
                jugador->actualizar_cola_ee(it->first, it->second);
            }
            if (it->second == 0) {
                it = entrenamientos_en_cola.erase(it);
            } else {
                ++it;
            }
        }
    }

    void actualizar_tropas(IJugador* jugador, int dt) {
        for (auto& it : jugadores.at(jugador).tropas) {
            Tropa* tropa = it.second;
            if (tropa->actualizar_camino(dt)) {
                broadcast([tropa] (IJugador* j) {
                    j->sincronizar_tropa(tropa->id, tropa->x_actual, 
                        tropa->y_actual);
                });
            }
            tropa->actualizar(dt);
        }
    }

    void broadcast(std::function<void(IJugador*)> mensaje) {
        for (auto& it : jugadores) {
            mensaje(it.first);
        }
    }
    
    struct Edificio {
        int id_edificio;
        int celda_x, celda_y;
        std::string clase;
        int id_propietario;
        Edificio(int id, int x, int y, int jugador, const std::string& clase_)
            : id_edificio(id), celda_x(x), celda_y(y), clase(clase_), 
              id_propietario(jugador)
        { }
    };

    struct Tropa {
        int id;
    
        int id_propietario;

        int velocidad = 16;

        int x_actual, y_actual;
        int x_destino, y_destino;
        float fx_actual, fy_actual;

        std::vector<std::pair<int, int>> camino_tropa;
        
        unsigned int paso_actual = 0;
        bool esta_en_camino = false;

        int vida;

        Tropa(int id_, int x, int y, int vida_, int propie)
            : id(id_), id_propietario(propie), vida(vida_)
        { 
            fx_actual = x_actual = x_destino = x;
            fy_actual = y_actual = y_destino = y;
        }

        bool llego_a(const std::pair<int, int>& posicion) {
            return (posicion.first == x_actual) && 
                   (posicion.second == y_actual);
        }

        // devuelve true si hay que sync.
        bool actualizar_camino(int) {
            if (!esta_en_camino)
                return false;
    
            bool retval = false;
            if (llego_a(camino_tropa[paso_actual])) {
                if (paso_actual > 0) {
                    retval = true;
                }
                
                paso_actual++;

                if (paso_actual >= camino_tropa.size()) {
                    esta_en_camino = false;
                } else {
                    x_destino = camino_tropa[paso_actual].first;
                    y_destino = camino_tropa[paso_actual].second;
                }
            }
            return retval;
        }

        void actualizar(int dt) {
            float vx = 0, vy = 0;

            if ((x_destino == x_actual) && (y_destino == y_actual))
                return;

            if (x_destino != x_actual)
                vx = (x_destino - x_actual) / abs(x_destino - x_actual);
            if (y_destino != y_actual)
                vy = (y_destino - y_actual) / abs(y_destino - y_actual);

            vx = vx / sqrt(vx*vx + vy*vy);
            vy = vy / sqrt(vx*vx + vy*vy);

            vx *= ((0.4 / 15) / 16) * velocidad;
            vy *= ((0.4 / 15) / 16) * velocidad;

            float dx = vx * dt,
                  dy = vy * dt;

            if (abs(dx) < abs(x_destino - fx_actual))
                fx_actual += dx;
            else
                fx_actual = x_destino;
                
            if (abs(dy) < abs(y_destino - fy_actual))
                fy_actual += dy;
            else
                fy_actual = y_destino;

            x_actual = round(fx_actual);
            y_actual = round(fy_actual);
        }
    };

    struct Jugador {
        bool listo = false;

        std::unordered_map<std::string, int> construcciones;
        std::unordered_set<std::string> construcciones_esperando_ubicacion;
        std::unordered_map<std::string, int> construcciones_en_cola;
        std::unordered_map<int, Edificio*> edificios;

        std::unordered_map<std::string, int> entrenamientos;
        std::unordered_map<std::string, int> entrenamientos_en_cola;
        std::unordered_map<int, Tropa*> tropas;

        int dinero = DINERO_INICIAL;
        int dinero_maximo = DINERO_MAXIMO_BASE;

        int energia = ENERGIA_INICIAL;
        int energia_maxima = ENERGIA_MAXIMA_BASE;

        Edificio& centro_construccion;

        Jugador(Edificio& centro_construccion_) 
        : centro_construccion(centro_construccion_)
        { }
    };

    bool terminado = false;
    bool b_partida_iniciada = false;
    
    std::unordered_map<IJugador*, Jugador> jugadores;
    std::unordered_map<int, Edificio> edificios_global;
    std::unordered_map<int, Tropa> tropas_global;

    nlohmann::json infraestructura, ejercito, mapa;

    int id_tropa = 0, id_edificio = 0;

    Edificio& crear_edificio(int x, int y, int jugador, const std::string& clase) {
        edificios_global.emplace(
            id_edificio,
            Edificio(id_edificio, x, y, jugador, clase)
        );
        return edificios_global.at(id_edificio++);
    }
};

} // namespace servidor

#endif // _MOCK_MODELO_H_