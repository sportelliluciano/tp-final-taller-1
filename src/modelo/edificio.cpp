#include "modelo/edificio.h"

#include "modelo/edificio_base.h"
#include "modelo/edificio_caracteristicas.h"

namespace modelo {

Edificio::Edificio(EdificioBase* info_base_):info_base(info_base_){
    
}
Edificio::Edificio(EdificioBase* info_base_,int x,int y):
                                caracteristicas(EdificioCaracteristicas(
                                    x,
                                    y,
                                    info_base->get_ptos_est())),
                                info_base(info_base_){}
Edificio::Edificio(){}
Edificio::~Edificio(){}

/*
Edificio& operator=(const Edificio& other){
    this->caracteristicas = other.caracteristicas;
    this->info_base = other.info_base;
}*/
Edificio Edificio::clonar(int x,int y){
    return Edificio(info_base,x,y);    
}
void Edificio::recibir_dano(unsigned int dano){
    if(!caracteristicas.reducir_ptos_est(dano)){
    //destruiredificio
    }
}
char Edificio::get_id(){
    return caracteristicas.get_id();    
}
}
