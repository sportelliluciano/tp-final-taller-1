#include "modelo/edificio_caracteristicas.h"

namespace modelo {
EdificioCaracteristicas::EdificioCaracteristicas(){
    
}
EdificioCaracteristicas::EdificioCaracteristicas(int id_,int x_,int y_,
                            unsigned int puntos_de_estructura_):
                            id(id_),
                            x(x_),
                            y(y_),
                            puntos_de_estructura(puntos_de_estructura_){
    //asignar id unico
}
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
int EdificioCaracteristicas::get_vida(){
    return puntos_de_estructura;
}
}
