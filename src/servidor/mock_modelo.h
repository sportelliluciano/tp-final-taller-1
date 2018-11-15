#ifndef _MOCK_MODELO_H_
#define _MOCK_MODELO_H_

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "libs/json.hpp"
#include "servidor/i_modelo.h"

namespace servidor {

using namespace conexion;

class MockModelo : public IModelo {
public:
    MockModelo() { }

    void crear_jugador(IJugador* nuevo_jugador) { 
        jugador = nuevo_jugador;
    }
    
    void iniciar_partida() {
        b_partida_iniciada = true;
    }

    bool partida_iniciada() const {
        return b_partida_iniciada;
    }

    bool juego_terminado() const {
        return terminado;
    }

    void jugador_desconectado(IJugador* jugador_act) {
        terminado = true;
    }

    bool iniciar_construccion_edificio(IJugador* jugador_act,
        const std::string& clase)
    {
        if (construcciones_en_cola.count(clase) == 0)
            construcciones_en_cola.insert({clase, 1});
        else
            construcciones_en_cola.at(clase)++;
        
        jugador_act->actualizar_cola_cc(clase, construcciones_en_cola.at(clase));
        return true;
    }
    
    bool cancelar_construccion_edificio(IJugador* jugador_act, 
        const std::string& clase)
    {
        if (construcciones_en_cola.count(clase) != 0) {
            construcciones_en_cola.at(clase)--;
            jugador->actualizar_cola_cc(clase, construcciones_en_cola.at(clase));
            if (construcciones_en_cola.at(clase) <= 0)
                construcciones_en_cola.erase(clase);
            
            return true;
        }
        return false;
    }
    
    bool ubicar_edificio(IJugador* jugador_act, int celda_x, int celda_y,
        const std::string& clase)
    {
        int id_edificio = edificios.size();
        edificios.emplace(
            id_edificio,
            Edificio(
                id_edificio,
                celda_x, celda_y,
                0,
                clase
            )
        );
        jugador->crear_edificio(
            id_edificio,
            clase, 
            celda_x, celda_y, 
            jugador->obtener_id()
        );
        construcciones_esperando_ubicacion.erase(clase);
        return true;
    }

    bool vender_edificio(IJugador* jugador_act, int id_edificio) {
        edificios.erase(id_edificio);
        jugador->eliminar_edificio(id_edificio);
        return true;
    }

    bool iniciar_entrenamiento_tropa(IJugador* jugador_act,
        const std::string& clase) 
    {
        return false;
    }

    bool cancelar_entrenamiento_tropa(IJugador* jugador_act,
        const std::string& clase)
    {
        return false;
    }

    bool mover_tropas(IJugador* jugador_act, const std::vector<int>& ids,
        int x_px, int y_px) 
    {
        x_destino = x_px;
        y_destino = y_px;
        
        for (int id : ids) {
            jugador->mover_tropa(id, { {x_tropa, y_tropa}, {x_px, y_px}});
        }

        return true;
    }

    bool atacar_tropa(IJugador* jugador_act, 
        const std::vector<int>& ids_atacantes, int id_atacado) 
    {
        return false;
    }

    bool indicar_especia_cosechadora(IJugador* jugador_act,
        const std::vector<int>& ids, int celda_x, int celda_y)
    {
        return false;
    }

    void actualizar(int dt) {
        if (!tropa_creada) {
            jugador->crear_tropa(0, "a", x_tropa, y_tropa, 100, 
                jugador->obtener_id());
            tropa_creada = true;
        }
        actualizar_construcciones(dt);
        if (wait <= 0 && !esta_en_camino && paso_actual == 0) {
            jugador->mover_tropa(0, camino_tropa);
            paso_actual = 0;
            x_destino = camino_tropa[paso_actual].first;
            y_destino = camino_tropa[paso_actual].second;
            esta_en_camino = true;
        } else {
            wait -= dt;
        }
        actualizar_tropas(dt);
    }

    ~MockModelo() { }

private:

    void actualizar_construcciones(int dt) {
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

    bool llego_a(const std::pair<int, int>& posicion) {
        return (abs(posicion.first - x_tropa) < 1) && 
               (abs(posicion.second - y_tropa) < 1);
    }

    void actualizar_tropas(int dt) {
        if (esta_en_camino) {
            if (llego_a(camino_tropa[paso_actual])) {
                if (paso_actual > 0)
                    jugador->sincronizar_tropa(0, x_tropa, y_tropa);
                
                paso_actual++;

                if (paso_actual >= camino_tropa.size()) {
                    esta_en_camino = false;
                } else {
                    x_destino = camino_tropa[paso_actual].first;
                    y_destino = camino_tropa[paso_actual].second;
                }
            }
        }

        float vx = 0, vy = 0;

        if (abs(x_destino - x_tropa) > 0) {
            vx = (x_destino - fx_tropa) / abs(x_destino - fx_tropa);
            vx *= 0.4 / 15;
        }

        if (abs(y_destino - y_tropa) > 0) {
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
    int x_tropa = 32, y_tropa = 32*5;
    float fx_tropa = x_tropa, fy_tropa = y_tropa, 
        x_destino = x_tropa, y_destino = y_tropa;
    
    //const std::vector<int> camino_tropa = {1*32,4*32,2*32,3*32,3*32,3*32,4*32,
    //    2*32,5*32,2*32,6*32,1*32,7*32,1*32};
    const std::vector<std::pair<int, int>> camino_tropa = 
        { {1*32,5*32}, {2*32,5*32}, {3*32,6*32}, {4*32,6*32},
          {5*32,7*32}, {6*32,7*32}, {7*32,7*32}, {8*32,8*32} };
    int paso_actual = 0;
    bool esta_en_camino = false;

    int wait = 5000;
    bool terminado = false;
    bool b_partida_iniciada = false;
    IJugador *jugador = nullptr;
};

} // namespace servidor

#endif // _MOCK_MODELO_H_