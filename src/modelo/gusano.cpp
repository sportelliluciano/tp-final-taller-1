#include "modelo/gusano.h"

namespace modelo { 

Gusano::Gusano(){

}
Gusano::~Gusano(){

}

bool Gusano::esperando_comer(){
    return tiempo_espera <= 0;
}
void Gusano::comer(){
    tiempo_espera = TIEMPO_ESPERA;
}
void Gusano::actualizar(int dt){
    tiempo_espera -= dt;
}
int Gusano::get_id(){
    return id;
}
}