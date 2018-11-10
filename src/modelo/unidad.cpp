#include "modelo/unidad.h"

#include "libs/json.hpp"

#include "modelo/arma.h"
#include "modelo/edificio.h"
#include "modelo/unidad_base.h"

namespace modelo {

Unidad::Unidad(int id_, int x, int y, UnidadBase& unidad_base_):
    id(id_),
    pos_x(x),
    pos_y(y),
    vida(unidad_base_.get_vida()),
    unidad_base(unidad_base_)
{
    //se le asigna un id unico
}
Unidad::~Unidad() {

}
void Unidad::mover(int x, int y){

}
void Unidad::recibir_dano(unsigned int dano){
    vida -= dano;
    //if (vida <= 0) destruir
}

void Unidad::atacar(Unidad& victima){
    unidad_base.atacar_a(&victima);
}

void Unidad::atacar(Edificio& edificio){
    unidad_base.atacar_a(&edificio);
}

int Unidad::get_id(){
    return id;
}
unsigned int Unidad::get_vida(){
    return vida;
}
}  // namespace modelo
