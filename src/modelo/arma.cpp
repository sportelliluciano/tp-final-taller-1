#include "modelo/arma.h"

#include "modelo/arma_base.h"
#include "modelo/arma_caracteristicas.h"
#include "modelo/unidad.h"
#include "modelo/edificio.h"

namespace modelo {

Arma::Arma(ArmaBase& info_base,ArmaCaracteristicas& caracteristicas):
                            info_base(info_base),
                            caracteristicas(caracteristicas){}

Arma::~Arma(){}

void Arma::atacar_a(Unidad* victima){

}

void Arma::atacar_a(Edificio& victima){

}

} // namespace modelo
