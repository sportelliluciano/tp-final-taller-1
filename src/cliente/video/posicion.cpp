#include "cliente/video/posicion.h"

namespace cliente {

Posicion::Posicion() : x(0), y(0) { }

Posicion::Posicion(int x_, int y_) : x(x_), y(y_) { }

bool Posicion::operator==(const Posicion& otro) const {
    return (x == otro.x) && (y == otro.y);
}

} // namespace cliente
