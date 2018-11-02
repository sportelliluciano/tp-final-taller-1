#ifndef _EDIFICIO_H_
#define _EDIFICIO_H_

#include "edificio_base.h"
#include "edificio_caracteristicas.h"

namespace modelo {

class Edificio{
    private:
    EdificioCaracteristicas caracteristicas;
    EdificioBase info_base;

    public:
    Edificio();
    ~Edificio()
    Edificio clonar(char id,int x,int y);
    void recibir_dano(unsigned int dano);

};
}
#endif