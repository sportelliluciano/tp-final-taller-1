#include "modelo/arma.h"

#include "modelo/arma_base.h"
//#include "modelo/arma_caracteristicas.h"
#include "modelo/unidad.h"
#include "modelo/edificio.h"

namespace modelo {

Arma::Arma(ArmaBase& info_base_):
                            arma_base(info_base_)
                            {}
/*
Arma::Arma(ArmaBase& info_base_,ArmaCaracteristicas& caracteristicas_):
                            arma_base(info_base_),
                            caracteristicas(caracteristicas_)
                            {}*/
//Arma::Arma(){}
Arma::~Arma(){}

int Arma::atacar_a(Atacable* victima){
    return arma_base.atacar_a(victima);
}
/*
void Arma::atacar_a(Edificio* victima){
    arma_base.atacar_a(victima);
}*/
unsigned int Arma::obtener_frecuencia(){
    return arma_base.obtener_frecuencia();
}
} // namespace modelo
