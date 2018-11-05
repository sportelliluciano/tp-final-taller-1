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
    char id;
    int pos_x;
    int pos_y;
    int vida;
    UnidadBase& unidad_base;
    Arma& arma;

public:
    Unidad(int pos_x,int pos_y, UnidadBase& unidad_base, Arma& arma);
    //Unidad(UnidadBase& unidad_base);
    ~Unidad();
    //void clonar(int x,int y);
    void moverse(int x, int y);
    void recibir_dano(unsigned int dano);
    void atacar(Unidad& victima);
    void atacar(Edificio& edificio);
    char get_id();
};

} // namespace modelo

#endif // _UNIDAD_H_
