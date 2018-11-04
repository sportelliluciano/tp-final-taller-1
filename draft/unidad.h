#ifndef _UNIDAD_H_
#define _UNIDAD_H_

#include "libs/json.hpp"
#include "edificio.h"
#include "unidadb_base.h"

namespace modelo {

class Unidad{
    private:
    char id;
    int pos_x;
    int pos_y;
    int vida;
    UnidadBase& unidad_base;

    public:
    Unidad(char id,int pos_x,int pos_y,
                            UnidadBase& unidad_base);
    //Unidad(UnidadBase& unidad_base);
    ~Unidad();
    clonar(char id,int x,int y);
    void moverse(int x, int y);
    void recibir_dano(unsigned int dano);
    void atacar(Unidad& victima);
    void atacar(Edificio& edificio);
    char get_id();
};
}
#endif