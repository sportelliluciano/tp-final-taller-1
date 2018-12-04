#include "modelo/edificio_caracteristicas.h"

namespace modelo {
EdificioCaracteristicas::EdificioCaracteristicas(){
}
EdificioCaracteristicas::EdificioCaracteristicas(int id_,int x_,int y_,
                            unsigned int puntos_de_estructura_):
                            id(id_),
                            posicion(Posicion(x_,y_)),
                            puntos_de_estructura(puntos_de_estructura_){
}
EdificioCaracteristicas::~EdificioCaracteristicas(){
}
int EdificioCaracteristicas::reducir_ptos_est(int dano){
    puntos_de_estructura -= dano;
    return puntos_de_estructura; 
}
int EdificioCaracteristicas::get_id(){
    return id;
}
int EdificioCaracteristicas::get_vida(){
    return puntos_de_estructura;
}
Posicion& EdificioCaracteristicas::get_posicion(){
    return posicion;
}
} // namespace modelo
