#include "modelo/celda.h"

namespace modelo {

Celda::Celda(int x_, int y_) {
    posicion_x = x_;
    posicion_y = y_;
    tipo_celda = 2;
}

int Celda::tipo() const {
    return tipo_celda;
}

void Celda::set_tipo(int tipo) {
    tipo_celda = tipo;
}

int Celda::x() const {
    return posicion_x;
}

int Celda::y() const {
    return posicion_y;
}

bool Celda::es_caminable() const {
    return tipo_celda != 0;
}

} // namespace modelo
