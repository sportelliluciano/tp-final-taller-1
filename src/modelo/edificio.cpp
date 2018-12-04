#include "modelo/edificio.h"

#include <string>
#include <utility>

#include "modelo/edificio_base.h"
#include "modelo/edificio_caracteristicas.h"

namespace modelo {

Edificio::Edificio(EdificioBase& info_base_):info_base(info_base_){
}
Edificio::Edificio(EdificioBase& info_base_,int id,int x,int y):
                                caracteristicas(EdificioCaracteristicas(
                                    id,
                                    x,
                                    y,
                                    info_base_.get_ptos_est())),
                                info_base(info_base_){
}
Edificio::~Edificio(){
}

Edificio Edificio::clonar(int id,int x,int y){
    return Edificio(info_base,id,x,y);    
}

int Edificio::recibir_dano(unsigned int dano){
    return caracteristicas.reducir_ptos_est(dano);
}

int Edificio::get_id() {
    return caracteristicas.get_id();    
}

int Edificio::get_vida(){//temporal
    return caracteristicas.get_vida();
}

unsigned int Edificio::get_costo(){
    return info_base.get_costo();
}

unsigned int Edificio::get_energia(){
    return info_base.get_energia();
}

std::pair<int,int>& Edificio::get_dimensiones(){
    return info_base.get_dimensiones();
}

Posicion& Edificio::get_posicion(){
    return caracteristicas.get_posicion();
}

unsigned int Edificio::get_tiempo(){
    return info_base.get_tiempo();
}

std::string& Edificio::get_tipo() const {
    return info_base.get_tipo();
}
} // namespace modelo
