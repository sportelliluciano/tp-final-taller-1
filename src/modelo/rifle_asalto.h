#ifndef _RIFLE_ASALTO_H_
#define _RIFLE_ASALTO_H_
#include "arma.h"

namespace modelo {

/**
 * \brief RifleAsalto. 
 * 
 * Representa un Rifle de Asalto, un tipo de modelo::Arma.
 */
class RifleAsalto : public Arma { 
public:
    RifleAsalto(int daño, int frecuencia_disparo);

    virtual int get_id();

    ~RifleAsalto();
};

} // namespace modelo

#endif // _RIFLE_ASALTO_H_