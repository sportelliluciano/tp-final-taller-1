#ifndef _ARMA_H_
#define _ARMA_H_

#include "modelo/arma_base.h"
#include "modelo/arma_caracteristicas.h"
#include "modelo/unidad.h"
#include "modelo/edificio.h"

namespace modelo {

class Arma{
    private:
    ArmaBase& info_base;
    ArmaCaracteristicas& caracteristicas;

    public:
    Arma(ArmaBase& info_base,ArmaCaracteristicas& caracteristicas);
    ~Arma();
    void atacar_a(Unidad& victima);
    void atacar_a(Edificio& victima);
};
}
#endif