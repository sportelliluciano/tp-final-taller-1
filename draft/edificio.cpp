#include "edificio.h"

#include "edificio_base.h"
#include "edificio_caracteristicas.h"

Edificio::Edificio(EdificioBase& info_base):info_base(info_base){}
Edificio::Edificio(EdificioBase& info_base,int x,int y):
                                caracteristicas(EdificioCaracteristicas(
                                    x,
                                    y,
                                    info_base.get_ptos_est())),
                                info_base(info_base){}
Edificio::~Edificio()
Edificio Edificio::clonar(int x,int y){
    return Edificio(EdificioBase& info_base,int x,int y)    
}
void Edificio::recibir_dano(unsigned int dano){
    if(!caracteristicas.reducir_ptos_est(dano)){
    //destruiredificio
    }
}
char Edificio::get_id(){
    return caracteristicas.get_id();    
}
