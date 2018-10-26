#ifndef _ARMA_H_
#define _ARMA_H_

namespace modelo {

/**
 * \brief Arma. 
 * 
 * Representa un arma utilizada por modelo::ActorArmado.
 */
class Arma { 
public:
    Arma(int daño, int frecuencia_disparo);

    virtual int get_id() = 0;

    int get_daño();

    int get_frecuencia_disparo();

    virtual ~Arma();

protected:
    int daño, frecuencia_disparo;
};

} // namespace modelo

#endif // _ARMA_H_