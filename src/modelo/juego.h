#ifndef _JUEGO_H_
#define _JUEGO_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "libs/json.hpp"

#include "modelo/id.h"
#include "modelo/jugador.h"
#include "modelo/unidad.h"
#include "modelo/infraestructura.h"
#include "modelo/ejercito.h"
#include "modelo/terreno.h"
#include "conexion/i_jugador.h"
#include "conexion/i_modelo.h"

namespace modelo {

class Juego : public IModelo {
    private:
    Terreno terreno;
    Infraestructura inf;
    Ejercito ejercito;
    std::unordered_map<int,Jugador> jugadores;
    Id id;
    bool empezo = false;
    void actualizar_construcciones(int dt) ;
    void actualizar_tropas(int dt);
    std::vector<IJugador*> comunicacion_jugadores;

    public:
    Juego();
    ~Juego();
    void inicializar(const nlohmann::json& mapa, 
        const nlohmann::json& edificios, const nlohmann::json& ejercito_);
    void iniciar_partida();
    bool partida_iniciada() const override;
    bool juego_terminado() const override;
    void crear_jugador(IJugador* jugador);
    void jugador_desconectado(IJugador* jugador);//no implementada
    bool iniciar_construccion_edificio(IJugador* jugador,
        const std::string& clase);
    bool cancelar_construccion_edificio(IJugador* jugador, 
        const std::string& clase);
    bool ubicar_edificio(IJugador* jugador, int celda_x, int celda_y,
        const std::string& clase);    
    bool vender_edificio(IJugador* jugador, int id_edificio);//sin implementar
    bool iniciar_entrenamiento_tropa(IJugador* jugador,
        const std::string& clase);
    bool cancelar_entrenamiento_tropa(IJugador* jugador,
        const std::string& clase);
    bool mover_tropas(IJugador* jugador, const std::vector<int>& ids,
                                    int x, int y);
    bool atacar_tropa(IJugador* jugador, 
        const std::vector<int>& ids_atacantes, int id_atacado);//le falta                                    
    bool indicar_especia_cosechadora(IJugador* jugador,
        const std::vector<int>& ids, int celda_x, int celda_y) override;//sin implementar
    void actualizar(int dt_ms);

    void destruir_unidad(int id_jugador,int id);
    void atacar_unidad(int id_jugador,int id_victima,int id_atacante);
    void atacar_edificio(int id_jugador,int id_edificio,int id_atacante);
    Unidad& get_unidad(int id);
};
}
#endif
