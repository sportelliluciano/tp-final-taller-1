#include "modelo/infraestructura_creador.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

#include "libs/json.hpp"
#include "modelo/edificio.h"
#include "modelo/edificio_base.h"

namespace modelo {

InfraestructuraCreador::InfraestructuraCreador(){
    using nlohmann::json;

    std::ifstream entrada("../data/edificios.json");

    json edificios_json;

    entrada >> edificios_json;

    auto it = edificios_json.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for(; it != edificios_json.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);
        info_base.emplace(elem["id"], EdificioBase(elem));
        prototipos.emplace(elem["id"], Edificio(info_base.at(elem["id"])));
    }
}
InfraestructuraCreador::~InfraestructuraCreador(){}
Edificio InfraestructuraCreador::clonar(std::string id_tipo,int id,int x,int y){
    return prototipos.at(id_tipo).clonar(id,x,y);
}
unsigned int InfraestructuraCreador::get_costo(std::string id_tipo){
    return info_base.at(id_tipo).get_costo();
}
unsigned int InfraestructuraCreador::get_energia(std::string id_tipo){
    return info_base.at(id_tipo).get_energia();
}
std::pair<int,int>& InfraestructuraCreador::get_dimensiones(std::string id_tipo)const{
    return info_base.at(id_tipo).get_dimensiones();
}
}
