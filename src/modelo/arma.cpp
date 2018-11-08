#include "modelo/arma.h"

#include "modelo/arma_base.h"
#include "modelo/arma_caracteristicas.h"
#include "modelo/unidad.h"
#include "modelo/edificio.h"

namespace modelo {

Arma::Arma(ArmaBase* info_base_,ArmaCaracteristicas* caracteristicas_):
                            arma_base(info_base_),
                            caracteristicas(caracteristicas_){}
Arma::Arma(){}
Arma::~Arma(){}

void Arma::atacar_a(Unidad* victima){
    arma_base->atacar_a(victima);
}

void Arma::atacar_a(Edificio* victima){
    arma_base->atacar_a(victima);
}

} // namespace modelo
