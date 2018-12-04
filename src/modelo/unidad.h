#ifndef _UNIDAD_H_
#define _UNIDAD_H_

#include <utility>
#include <vector>
#include <string>

#include "libs/json.hpp"

#include "modelo/atacable.h"
#include "modelo/arma.h"
#include "modelo/edificio.h"
#include "modelo/unidad_base.h"
#include "modelo/posicion.h"
#include "modelo/terreno.h"

namespace modelo {
/**
 * \brief Unidad. 
 * 
 * Id: identificar unico dentro del juego.
 * Unidad_base: informacion general del tipo de unidad.
 * Posicion: posicion dentro del terreno donde se ubica la unidad.
 */
class Unidad: public Atacable {
private:
    int id;
    int vida;
    int tiempo_para_atacar = 0;
    bool atacando = false;
    Atacable* victima = nullptr;
    int id_victima_ =-1;

protected:
    UnidadBase& unidad_base;
    std::vector<Posicion> camino; 
    Posicion posicion;
    bool esta_en_camino = false;
    unsigned int paso_actual = 0;

public:
    Unidad(int id,int pos_x,int pos_y, UnidadBase& unidad_base);
    ~Unidad();
    /**
     * setea toda la informacion para poder atacar a victima.
     * devuelve true si se cumplen todas las conidciones para poder atacar,
     * false de lo contrario.
     */
    bool configurar_ataque(Atacable* victima);
    /**
     * ejecuta el ataque hacia victima.
     */
    int atacar(Atacable* victima);
    /**
     * reduce la vida de la unidad en unidades igual a dano
     */
    int recibir_dano(unsigned int dano);
    /**
     * desconfigura el ataque a victima.
     */
    void parar_ataque();
    /**
     * setea toda la informacion para poder moverse.
     */
    virtual void configurar_camino(const std::vector<Posicion>& nuevo_camino);
    /**
     * devuelve true si la unidad esta en movimeinto.
     */
    virtual bool en_movimiento() const;
    /**
     * Devuelve true si hay que sincronizar con los jugadores
     */
    virtual bool actualizar_posicion(int dt,Terreno* terreno);
    virtual int actualizar_ataque(int dt,Terreno* terreno);
    
    std::string& get_clase() const;
    bool esta_atacando();
    int id_victima();
    int get_id();
    unsigned int get_vida();
    std::pair<int,int>& get_dimensiones();
    Posicion& get_posicion();    
};

} // namespace modelo

#endif // _UNIDAD_H_
