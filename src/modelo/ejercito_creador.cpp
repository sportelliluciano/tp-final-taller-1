#include "modelo/ejercito_creador.h"

#include <iostream>
#include <string>
#include <fstream> 
#include <unordered_map>

#include "libs/json.hpp"
#include "modelo/unidad.h"
#include "terreno.h"

namespace modelo {

EjercitoCreador::EjercitoCreador(){
}
void EjercitoCreador::inicializae(Terreno* terreno_,const nlohmann::json& ejercito){
    terreno = terreno_;
    using nlohmann::json;

    auto it = ejercito.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for(; it != ejercito.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);
        if(!armamento.tiene(elem["id_arma"]))continue;
        prototipos_base.emplace(elem["id"], UnidadBase(elem,armamento.get(elem["id_arma"])));
    }
}
EjercitoCreador::~EjercitoCreador(){}
Unidad EjercitoCreador::clonar(std::string id_tipo,int id,int x,int y){
    return Unidad(id,x,y,prototipos_base.at(id_tipo));
}
Cosechadora EjercitoCreador::clonar(std::string id_tipo,int id,int x,int y,Terreno* terreno_,IJugador* jugador){
    return Cosechadora(id,x,y,prototipos_base.at(id_tipo),terreno_,jugador);
}
unsigned int EjercitoCreador::get_costo(std::string id_tipo){
    return prototipos_base.at(id_tipo).get_costo();
}
unsigned int EjercitoCreador::get_vida(std::string id_tipo){
    return prototipos_base.at(id_tipo).get_vida();
}
std::pair<int,int>& EjercitoCreador::get_dimensiones(std::string id_tipo){
    return prototipos_base.at(id_tipo).get_dimensiones();
}
unsigned int EjercitoCreador::get_tiempo(std::string id_tipo){
    return prototipos_base.at(id_tipo).get_tiempo();

}
}