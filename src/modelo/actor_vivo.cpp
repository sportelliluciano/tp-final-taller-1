#include "modelo/actor_vivo.h"
using modelo::ActorVivo;
using modelo::Actor;

ActorVivo::ActorVivo(int vida, int costo, int pos_x, int pos_y) : Actor(pos_x, pos_y),
    vida(vida), costo(costo) {}

int ActorVivo::get_vida() const {
    return this->vida;
}

int ActorVivo::get_costo() const {
    return this->costo;
}

void ActorVivo::recibir_ataque(int ataque) {
    this->vida -= ataque;
}

bool ActorVivo::esta_vivo() const {
    return this->vida > 0;
}

ActorVivo::~ActorVivo() {}