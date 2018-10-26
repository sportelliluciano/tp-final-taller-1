#include "modelo/actor_armado.h"
using modelo::ActorArmado;
using modelo::ActorVivo;

ActorArmado::ActorArmado(char casa, char rango, int tiempo_creacion, 
        int velocidad_movimento, int id_actor_requerido, int vida, 
        int costo, int pos_x, int pos_y) : ActorVivo(vida, costo, pos_x, pos_y),
        casa(casa), rango(rango), tiempo_creacion(tiempo_creacion), 
        velocidad_movimento(velocidad_movimento), id_actor_requerido(id_actor_requerido) {}

void ActorArmado::atacar_a_actor_vivo(modelo::ActorVivo* actor_a_atacar) {
    actor_a_atacar->recibir_ataque(this->arma->get_poder_ataque());
}

ActorArmado::~ActorArmado() {}