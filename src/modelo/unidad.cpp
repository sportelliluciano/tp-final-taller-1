#include "modelo/unidad.h"

#include "libs/json.hpp"

#include "modelo/arma.h"
#include "modelo/edificio.h"
#include "modelo/unidad_base.h"

namespace modelo {

Unidad::Unidad(int x, int y, UnidadBase& unidad_base_, Arma& arma_):
    pos_x(x),
    pos_y(y),
    vida(unidad_base.get_vida()),
    unidad_base(unidad_base_),
    arma(arma_)
{
    //se le asigna un id unico
}

Unidad::~Unidad() {

}

void Unidad::moverse(int x, int y){

}

void Unidad::recibir_dano(unsigned int dano){
    vida -= dano;
    //if (vida <= 0) destruir
}

void Unidad::atacar(Unidad& victima){
    arma.atacar_a(&victima);
}

void Unidad::atacar(Edificio& edificio){
    arma.atacar_a(edificio);
}

char Unidad::get_id(){
    return id;
}

}  // namespace modelo
