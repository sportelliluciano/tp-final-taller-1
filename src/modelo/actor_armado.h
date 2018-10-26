#ifndef _ACTOR_ARMADO_H_
#define _ACTOR_ARMADO_H_
#include "modelo/actor_vivo.h"
#include "modelo/actor.h"
#include "modelo/arma.h"

namespace modelo {

/**
 * \brief Actor armado. 
 * 
 * Representa una unidad armada.
 */
class ActorArmado : public ActorVivo { 
public:
    ActorArmado(char casa, char rango, int tiempo_creacion, 
        int velocidad_movimento, int id_actor_requerido, int vida, 
        int costo, int pos_x, int pos_y);

    void atacar_a_actor_vivo(modelo::ActorVivo* actor_a_atacar);

    virtual int get_id() = 0;

    virtual ~ActorArmado();

protected:
    char casa, rango;
    int tiempo_creacion, velocidad_movimento;
    int id_actor_requerido;
    modelo::Arma* arma;
};

} // namespace modelo

#endif // _ACTOR_ARMADO_H_