#include "modelo/arma.h"
using modelo::Arma;

Arma::Arma(int poder_ataque, int frecuencia_disparo) : poder_ataque(poder_ataque),
    frecuencia_disparo(frecuencia_disparo) {}

int Arma::get_poder_ataque() {
    return this->poder_ataque;
}

int Arma::get_frecuencia_disparo() {
    return this->frecuencia_disparo;
}

Arma::~Arma() {}