#ifndef _ACTOR_VIVO_H_
#define _ACTOR_VIVO_H_
#include "modelo/actor.h"

namespace modelo {

/**
 * \brief Actores con vida. 
 * 
 * Representa un actor del juego que tiene vida.
 */
class ActorVivo : public Actor { 
public:
    ActorVivo(int vida, int costo, int pos_x, int pos_y);

    int get_vida() const;
    int get_costo() const;

    void recibir_ataque(int ataque);
    bool esta_vivo() const;

    virtual int get_id() = 0;

    virtual ~ActorVivo();

protected:
    int vida, costo;
};

} // namespace modelo

#endif // _ACTOR_VIVO_H_