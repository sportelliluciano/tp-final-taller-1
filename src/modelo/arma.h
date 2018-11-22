#ifndef _ARMA_H_
#define _ARMA_H_

// TODO: Arreglar esto
namespace modelo { class Arma; }

#include "modelo/arma_base.h"
//#include "modelo/arma_caracteristicas.h"
#include "modelo/unidad.h"
#include "modelo/edificio.h"

namespace modelo {

class Arma {
private:
    ArmaBase& arma_base;
    //ArmaCaracteristicas& caracteristicas;

public:
    //Arma();
    Arma(ArmaBase& info_base);
    //Arma(ArmaBase& info_base, ArmaCaracteristicas& caracteristicas);
    ~Arma();
    int atacar_a(Atacable* victima);
    //void atacar_a(Edificio* victima);
    unsigned int obtener_frecuencia();
};

} // namespace modelo

#endif // _ARMA_H_