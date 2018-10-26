#include "modelo/rifle_asalto.h"
#include "modelo/arma.h"
#define ID_RIFLE_ASALTO 1;
using modelo::RifleAsalto;
using modelo::Arma;

RifleAsalto::RifleAsalto(int daño, int frecuencia_disparo) : 
    Arma(daño, frecuencia_disparo) {}

int RifleAsalto::get_id() {
    return ID_RIFLE_ASALTO;
}

RifleAsalto::~RifleAsalto() {}