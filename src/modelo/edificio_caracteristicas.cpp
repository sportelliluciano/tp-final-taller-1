#include "modelo/edificio_caracteristicas.h"

namespace modelo {
EdificioCaracteristicas::EdificioCaracteristicas(){
    
}
EdificioCaracteristicas::EdificioCaracteristicas(int x_,int y_,
                            unsigned int puntos_de_estructura_):
                            x(x_),
                            y(y_),
                            puntos_de_estructura(puntos_de_estructura_){}
EdificioCaracteristicas::~EdificioCaracteristicas(){

}
bool EdificioCaracteristicas::reducir_ptos_est(int dano){
    puntos_de_estructura -= dano;
    if (puntos_de_estructura > 0) return true;
    return false; 
}
int EdificioCaracteristicas::get_id(){
    return id;
}
}
