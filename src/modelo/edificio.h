#ifndef _EDIFICIO_H_
#define _EDIFICIO_H_

#include "modelo/edificio_base.h"
#include "modelo/edificio_caracteristicas.h"

namespace modelo {

class Edificio{
    private:
    EdificioCaracteristicas caracteristicas;
    EdificioBase& info_base;

    public:
    Edificio(EdificioBase& info_base);
    Edificio(EdificioBase& info_base,int x,int y);
    ~Edificio();
    Edificio clonar(int x,int y);
    void recibir_dano(unsigned int dano);
    char get_id();

};
}
#endif
