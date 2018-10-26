#include "modelo/actor.h"
using modelo::Actor;

Actor::Actor(int pos_x, int pos_y) : posicion_x(pos_x), posicion_y(pos_y) {}

int Actor::x() const {
    return this->posicion_x;
}

int Actor::y() const {
    return this->posicion_y;
}

Actor::~Actor() {}
