#include "modelo/celda.h"

namespace modelo {

Celda::Celda(tipo_celda_t tipo,int x_, int y_) {
    posicion_x = x_;
    posicion_y = y_;
    tipo_celda = tipo;
}

int Celda::tipo() const {
    return tipo_celda;
}

int Celda::x() const {
    return posicion_x;
}

int Celda::y() const {
    return posicion_y;
}

bool Celda::tiene_edificio() const {
    return edificio;
}

void Celda::agregar_edificio() {
    edificio = true;
}

void Celda::eliminar_edificio() {
    edificio = false;
}

bool Celda::es_caminable() const {
    return (tipo_celda == CELDA_ROCA || tipo_celda == CELDA_ARENA ||
            tipo_celda == CELDA_DUNA || tipo_celda == CELDA_ESPECIA) &&
           (!hay_tropa() && !tiene_edificio());
}

bool Celda::es_construible() {
    return (tipo_celda == CELDA_ROCA) && (!hay_tropa() && !tiene_edificio());
}

bool Celda::hay_tropa() const {
    return tropa;
}

void Celda::agregar_tropa() {
    tropa = true;
}

void Celda::eliminar_tropa() {
    tropa = false;
}

} // namespace modelo
