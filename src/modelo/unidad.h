#ifndef _UNIDAD_H_
#define _UNIDAD_H_

#include "libs/json.hpp"

// TODO: Arreglar esto
namespace modelo { class Unidad; }

#include "modelo/arma.h"
#include "modelo/edificio.h"
#include "modelo/unidad_base.h"
#include "modelo/posicion.h"
#include "modelo/terreno.h"

namespace modelo {

class Unidad {
private:
    int id;
    int vida;
    //UnidadBase& unidad_base;
    int tiempo_para_atacar = 0;
    bool atacando = false;
    Unidad* victima = nullptr;

protected:
    UnidadBase& unidad_base;
    std::vector<Posicion> camino; 
    Posicion posicion;
    bool esta_en_camino = false;
    unsigned int paso_actual = 0;

public:
    Unidad(int id,int pos_x,int pos_y, UnidadBase& unidad_base);
    ~Unidad();
    int recibir_dano(unsigned int dano);
    int atacar(Unidad* victima);
    void atacar(Edificio& edificio);
    int get_id();
    unsigned int get_vida();
    std::pair<int,int>& get_dimensiones();
    Posicion& get_posicion();
    bool llego_a(Posicion& posicion_);
    void configurar_camino(std::vector<Posicion> nuevo_camino);
    bool en_movimiento();
    /**
     * Devuelve true si hay que sincronizar con los jugadores
     */
    virtual bool actualizar_posicion(int dt,Terreno* terreno);
    virtual bool actualizar_ataque(int dt,Terreno* terreno);
    void configurar_ataque(Unidad* victima);
    std::string& get_clase() const;
    bool esta_atacando();
    void parar_ataque();
    int id_victima();
};

} // namespace modelo

#endif // _UNIDAD_H_
