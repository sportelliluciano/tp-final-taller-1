#ifndef _JUEGO_H_
#define _JUEGO_H_

#include <string>
#include <unordered_map>
#include <unordered_set>
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
#include "modelo/broadcaster.h"

namespace modelo {

class Juego : public IModelo {
    private:
    Broadcaster comunicacion_jugadores;
    Terreno terreno;
    Infraestructura inf;
    Ejercito ejercito;
    std::unordered_map<int,Jugador> jugadores;
    Id id;
    // Posiciones de los centros de construcción
    std::vector<std::vector<int>> posiciones_centros;
    bool empezo = false;
    void actualizar_construcciones(int dt);
    void actualizar_tropas(int dt);

public:
    /**** Métodos de interacción con el servidor. ****/
    Juego();

    void inicializar(const nlohmann::json& mapa, 
        const nlohmann::json& edificios, const nlohmann::json& ejercito_);
    
    void crear_jugador(IJugador* jugador) override;
    
    void iniciar_partida() override;

    void actualizar(int dt_ms);
    
    bool partida_terminada() const override;
    
    void jugador_desconectado(IJugador* jugador);

    virtual ~Juego();

    /**** Métodos que son llamados desde los clientes. *****/
    void jugador_listo(IJugador* jugador);

    void iniciar_construccion_edificio(IJugador* jugador,
        const std::string& clase);

    void cancelar_construccion_edificio(IJugador* jugador, 
        const std::string& clase);

    void ubicar_edificio(IJugador* jugador, int celda_x, int celda_y,
        const std::string& clase);

    void vender_edificio(IJugador* jugador, int id_edificio); //sin implementar
    
    void iniciar_entrenamiento_tropa(IJugador* jugador,
        const std::string& clase);

    void cancelar_entrenamiento_tropa(IJugador* jugador,
        const std::string& clase);

    void mover_tropas(IJugador* jugador, const std::unordered_set<int>& ids,
        int x, int y);

    void atacar_tropa(IJugador* jugador, 
        const std::unordered_set<int>& ids_atacantes, int id_atacado); //le falta
    
    void indicar_especia_cosechadora(IJugador* jugador,
        const std::unordered_set<int>& ids, int celda_x, int celda_y) override; //sin implementa

    /*** Otros métodos ****/
    void destruir_unidad(int id_jugador,int id);
    void atacar_unidad(int id_jugador,int id_victima,int id_atacante);
    void atacar_edificio(int id_jugador,int id_edificio,int id_atacante);
    Unidad& get_unidad(int id);
};
}
#endif
