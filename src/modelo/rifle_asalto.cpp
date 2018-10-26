#include "rifle_asalto.h"
#include "arma.h"
#define ID_RIFLE_ASALTO 1;
using modelo::RifleAsalto;
using modelo::Arma;

RifleAsalto::RifleAsalto(int poder_ataque, int frecuencia_disparo) : 
    Arma(poder_ataque, frecuencia_disparo) {}

int RifleAsalto::get_id() {
    return ID_RIFLE_ASALTO;
}

RifleAsalto::~RifleAsalto() {}