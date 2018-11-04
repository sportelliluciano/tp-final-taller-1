#include "unidad.h"

#include "libs/json.hpp"
#include "edificio.h"
#include "unidad_base.h"

Unidad(int pos_x,int pos_y,
                        UnidadBase& unidad_base):
                        pos_x(pos_x),
                        pos_y(pos_y),
                        vida(unidad_base.get_vida()),
                        unidad_base(unidad_base){}
Unidad::~Unidad();
void Unidad::moverse(int x, int y){

}
void Unidad::recibir_dano(unsigned int dano){
    vida -= dano;
    //if (vida <= 0) destruir
}
void Unidad::atacar(Unidad& victima){
    unidad_base.atacar(victima);
}
void Unidad::atacar(Edificio& edificio){
    unidad_base.atacar(edificio);
}
char Unidad::get_id(){
    return id;
}
