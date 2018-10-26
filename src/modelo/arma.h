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
    Arma(int poder_ataque, int frecuencia_disparo);

    virtual int get_id() = 0;

    int get_poder_ataque();

    int get_frecuencia_disparo();

    virtual ~Arma();

protected:
    int poder_ataque, frecuencia_disparo;
};

} // namespace modelo

#endif // _ARMA_H_