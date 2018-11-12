#ifndef _UNIDAD_H_
#define _UNIDAD_H_

#include "libs/json.hpp"

// TODO: Arreglar esto
namespace modelo { class Unidad; }

#include "modelo/arma.h"
#include "modelo/edificio.h"
#include "modelo/unidad_base.h"
#include "modelo/posicion.h"

namespace modelo {

class Unidad {
private:
    int id;
    Posicion posicion;
    int vida;
    UnidadBase& unidad_base;

public:
    Unidad(int id,int pos_x,int pos_y, UnidadBase& unidad_base);
    ~Unidad();
    void mover(int x, int y);
    void recibir_dano(unsigned int dano);
    void atacar(Unidad& victima);
    void atacar(Edificio& edificio);
    int get_id();
    unsigned int get_vida();
    std::pair<int,int>& get_dimensiones();
    Posicion& get_posicion();
    unsigned int x();
    unsigned int y();
};

} // namespace modelo

#endif // _UNIDAD_H_
