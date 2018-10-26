#ifndef _CAÑON_H_
#define _CAÑON_H_
#include "arma.h"

namespace modelo {

/**
 * \brief Cañon. 
 * 
 * Representa un cañon, un tipo de modelo::Arma.
 */
class Cañon : public Arma { 
public:
    Cañon(int daño, int frecuencia_disparo);

    virtual int get_id();

    ~Cañon();
};

} // namespace modelo

#endif // _CAÑON_H_