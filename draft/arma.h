#ifndef _ARMA_H_
#define _ARMA_H_

#include "arma_base.h"
#include "arma_caracteristicas.h"
#include "unidad.h"

namespace modelo {

class Arma{
    private:
    ArmaBase info_base;
    ArmaCaracteristicas caracteristicas;

    public:
    void atacar_a(Unidad& victima);
};
}
#endif