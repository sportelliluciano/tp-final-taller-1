#ifndef _INFRAESTRUCTURA_H_
#define _INFRAESTRUCTURA_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "modelo/edificio.h"
#include "modelo/infraestructura_creador.h"
#include "modelo/terreno.h"
#include "modelo/id.h"
#include "comun/i_jugador.h"

#include "modelo/broadcaster.h"

namespace modelo {
/**
 * \brief Infraestructura. 
 * Se encarga de gestionar y almacenar todas los edificios dentro del juego.
 * Terreno: puntero al terreno(unica instancia) del juego.
 * Edificios: almacena todas los edificos del juego, con el formato 
 * id:Edificio.
 * Prototipos: instancia de InfraestructuraCreador(encargada de crear 
 * los edificios).
 */
class Infraestructura{
    private:
    Broadcaster& comunicacion_jugadores;
    Terreno* terreno;
    std::unordered_map<int,Edificio> edificios;
    InfraestructuraCreador prototipos;
    Id& id_;

    public:
    Infraestructura(Broadcaster& broadcaster,Id& id);
    /**
     * \brief Inicializador.
     *  
     * Inicializa todos los prototipos a partir de edificios, parametro en formato JSON.
     */
    void inicializar(Terreno* mapa,const nlohmann::json& edificios);
    /**
     * \brief crea una unidad.
     *  
     * crea una instancia de edificio de tipo igaul a id_tipo, en la celda 
     * con coordenadas (x,y).
     */
    int crear(const std::string& id_tipo,int x,int y, int id_propietario);
    /**
     * \brief Crea el centro de construcción para un jugador.
     */
    int crear_centro_construccion(int x, int y, int id_propietario);
    /**
     * \brief Elimina al edificio con id pasado por parametro del juego,
     * devuelve el monto de plata que se le devuelve al jugador
     * (es un porcentaje del costo del edificio determinado en FACTOR).
     */
    unsigned int reciclar(int id,int id_jugador);
    /**
     * \brief Elimina al edificio con id pasado por parametro del juego,
     */
    void destruir(int id,int id_jugador);
    Edificio& get(int id);
    unsigned int get_costo(std::string id_tipo);
    unsigned int get_energia(std::string id_tipo);
    unsigned int get_costo(int id);
    unsigned int get_energia(int id);
    unsigned int get_tiempo(int id);
    Posicion& get_posicion(const std::string& clase);
    bool pertenece(int id);
    void actualizar_edificios(int dt_ms);
};

} // namespace modelo

#endif // _INFRAESTRUCTURA_H_
