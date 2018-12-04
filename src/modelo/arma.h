#ifndef _ARMA_H_
#define _ARMA_H_

namespace modelo { class Arma; }

#include "modelo/arma_base.h"
#include "modelo/unidad.h"
#include "modelo/edificio.h"
//#include "modelo/arma_caracteristicas.h"

namespace modelo {
/**
 * \brief Arma
 * 
 * Arma de una unidad.
 * Encapsula todo la informacion sobre los ataques. 
 */
class Arma {
private:
    ArmaBase& arma_base;
    //ArmaCaracteristicas& caracteristicas;

public:
    /**
     * \brief Constructor.
     */
    explicit Arma(ArmaBase& info_base);
    //Arma(ArmaBase& info_base, ArmaCaracteristicas& caracteristicas);
    ~Arma();
    /**
     * \brief Ejecuta el ataque de la unidad posedora del arma
     * a la victima recibida por parametro..
     */
    int atacar_a(Atacable* victima);
    unsigned int obtener_frecuencia();
};

} // namespace modelo

#endif // _ARMA_H_
