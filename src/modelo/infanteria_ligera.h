#ifndef _INFANTERIA_LIGERA_H_
#define _INFANTERIA_LIGERA_H_
#include "actor_armado.h"
#include "actor.h"
#include "arma.h"

namespace modelo {

/**
 * \brief Actor armado. 
 * 
 * Representa una unidad armada llamada infanteria ligera.
 */
class InfanteriaLigera : public ActorArmado { 
public:
    InfanteriaLigera(char casa, char rango, int tiempo_creacion, 
        int velocidad_movimento, int id_actor_requerido, int vida, 
        int costo, int pos_x, int pos_y);

    virtual int get_id();

    ~InfanteriaLigera();
};

} // namespace modelo

#endif // _INFANTERIA_LIGERA_H_