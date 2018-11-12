#include "modelo/unidad.h"

#include "libs/json.hpp"

#include "modelo/arma.h"
#include "modelo/edificio.h"
#include "modelo/unidad_base.h"
#include "modelo/posicion.h"

namespace modelo {

Unidad::Unidad(int id_, int x, int y, UnidadBase& unidad_base_):
    id(id_),
    posicion(Posicion(x,y)),
    vida(unidad_base_.get_vida()),
    unidad_base(unidad_base_)
{
    //se le asigna un id unico
}
Unidad::~Unidad() {

}
void Unidad::mover(int x, int y){
    posicion.actualizar(x,y);
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
std::pair<int,int>& Unidad::get_dimensiones() {
    return unidad_base.get_dimensiones();
}
Posicion& Unidad::get_posicion(){
    return posicion;
}
unsigned int Unidad::x(){
    return posicion.x();
}
unsigned int Unidad::y(){
    return posicion.y();
}
}  // namespace modelo
