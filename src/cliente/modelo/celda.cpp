#include "cliente/modelo/celda.h"

namespace cliente {

Celda::Celda(int tile_no, int x_, int y_) {
    posicion_x = x_;
    posicion_y = y_;
    numero_tile = tile_no;
}

int Celda::numero_mosaico() const {
    return numero_tile;
}

int Celda::x() const {
    return posicion_x;
}

int Celda::y() const {
    return posicion_y;
}

bool Celda::contiene_tropas() const {
    return false;
}

std::vector<Tropa*> Celda::obtener_tropas() const {
    return std::vector<Tropa*>();
}

} // namespace cliente
