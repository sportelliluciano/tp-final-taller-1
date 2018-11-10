#ifndef _UNIDAD_H_
#define _UNIDAD_H_

#include "libs/json.hpp"

// TODO: Arreglar esto
namespace modelo { class Unidad; }

#include "modelo/arma.h"
#include "modelo/edificio.h"
#include "modelo/unidad_base.h"

namespace modelo {

class Unidad {
private:
    int id;
    int pos_x;
    int pos_y;
    int vida;
    UnidadBase& unidad_base;

public:
    //Unidad();
    Unidad(int id,int pos_x,int pos_y, UnidadBase& unidad_base);
    //Unidad(UnidadBase& unidad_base);
    ~Unidad();
    //void clonar(int x,int y);
    void mover(int x, int y);
    void recibir_dano(unsigned int dano);
    void atacar(Unidad& victima);
    void atacar(Edificio& edificio);
    int get_id();
    unsigned int get_vida();
};

} // namespace modelo

#endif // _UNIDAD_H_
