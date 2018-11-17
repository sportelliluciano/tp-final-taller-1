#ifndef _UNIDAD_H_
#define _UNIDAD_H_

#include "libs/json.hpp"

// TODO: Arreglar esto
namespace modelo { class Unidad; }

#include "modelo/arma.h"
#include "modelo/edificio.h"
#include "modelo/unidad_base.h"
#include "modelo/posicion.h"
#include "conexion/i_jugador.h"
#include "modelo/terreno.h"

namespace modelo {

class Unidad {
private:
    int id;
    Posicion posicion;
    int vida;
    UnidadBase& unidad_base;
    std::vector<Posicion> camino;
    bool esta_en_camino = false;
    unsigned int paso_actual = 0; 

public:
    Unidad(int id,int pos_x,int pos_y, UnidadBase& unidad_base);
    ~Unidad();
    void recibir_dano(unsigned int dano);
    void atacar(Unidad& victima);
    void atacar(Edificio& edificio);
    int get_id();
    unsigned int get_vida();
    std::pair<int,int>& get_dimensiones();
    Posicion& get_posicion();
    bool llego_a(Posicion& posicion_);
    void configurar_camino(std::vector<Posicion> nuevo_camino);
    bool en_movimiento();
    void actualizar_posicion(int dt,IJugador* jugador,Terreno terreno);
};

} // namespace modelo

#endif // _UNIDAD_H_
