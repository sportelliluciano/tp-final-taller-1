#ifndef _UNIDAD_H_
#define _UNIDAD_H_

#include "edificio.h"
#include "arma.h"

namespace modelo {

class Unidad{
    private:
    Arma& arma;
    
    public:
    Unidad(Arma& arma);
    ~Unidad();
    clonar(char id,int x,int y);
    void moverse(int x, int y);
    void recibir_dano(unsigned int dano);
    void atacar(Unidad& victima);
    void atacar(Edificio& edificio);
};
}
#endif