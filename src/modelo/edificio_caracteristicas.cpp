#include "modelo/EdificioCaracteristicas"

namespace modelo {

EdificioCaracteristicas::EdificioCaracteristicas(int x,int y,
                            unsigned int puntos_de_estructura):
                            x(x),
                            y(y),
                            puntos_de_estructura(puntos_de_estructura){}
~EdificioCaracteristicas();
bool reducir_ptos_est(dano){
    puntos_de_estructura -= dano;
    if (puntos_de_estructura > 0) return true;
    return false; 
}
char get_id(){
    return id;
}
}
