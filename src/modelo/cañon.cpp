#include "modelo/cañon.h"
#include "modelo/arma.h"
#define ID_CAÑON 2;
using modelo::Cañon;
using modelo::Arma;

Cañon::Cañon(int daño, int frecuencia_disparo) : Arma(daño, frecuencia_disparo) {}

int Cañon::get_id() {
    return ID_CAÑON;
}

Cañon::~Cañon() {}