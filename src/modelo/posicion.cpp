#include "posicion.h"

#include <cmath>

namespace modelo {

Posicion::Posicion(float x, float y) : x_(x), y_(y) { }

Posicion Posicion::operator+(const Posicion& otro) const {
    Posicion resultado(x_ + otro.x_, y_ + otro.y_);
    return resultado;
}

bool Posicion::operator==(const Posicion& otro) const {
    if (abs(x_ - otro.x_) > POSICION_CMP_TOLERANCIA)
        return false;
    if (abs(y_ - otro.y_) > POSICION_CMP_TOLERANCIA)
        return false;
    
    return true;
}

bool Posicion::operator!=(const Posicion& otro) const {
    return !operator==(otro);
}

float Posicion::distancia_a(const Posicion& otro) const {
    float delta_x, delta_y;
    delta_x = x_ - otro.x_;
    delta_y = y_ - otro.y_;
    return sqrt((delta_x * delta_x) + (delta_y * delta_y));
}

float Posicion::x() const {
    return x_;
}

float Posicion::y() const {
    return y_;
}
void Posicion::actualizar(float x,float y){
    x_ = x;
    y_ = y;
}

} // namespace modelo
