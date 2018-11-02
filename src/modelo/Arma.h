#ifndef _ARMA_H_
#define _ARMA_H_

#include "modelo/Unidad.h"

namespace modelo {

class Arma{
    public:
    virtual void atacar_a(char id_soldado_tipo, Unidad& victima)=0;
};
}
#endif