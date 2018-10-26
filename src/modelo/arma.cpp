#include "modelo/arma.h"
using modelo::Arma;

Arma::Arma(int poder_ataque, int frecuencia_disparo) : daño(daño),
    frecuencia_disparo(frecuencia_disparo) {}

int Arma::get_daño() {
    return this->daño;
}

int Arma::get_frecuencia_disparo() {
    return this->frecuencia_disparo;
}

Arma::~Arma() {}