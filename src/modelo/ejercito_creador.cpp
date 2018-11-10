#include "modelo/ejercito_creador.h"

#include <iostream>
#include <string>
#include <fstream> 
#include <unordered_map>

#include "libs/json.hpp"
#include "modelo/unidad.h"
#include "terreno.h"

namespace modelo {

EjercitoCreador::EjercitoCreador(Terreno& terreno_):terreno(terreno_){
    using nlohmann::json;

    std::ifstream entrada("../data/ejercito.json");

    json edificios_json;

    entrada >> edificios_json;

    auto it = edificios_json.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for(; it != edificios_json.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);
        if(!armamento.tiene(elem["id_arma"]))continue;
        std::cout << "se crea unidad base: " << elem["id"] << '\n';
        std::cout << "con arma: " << elem["id_arma"] << '\n';
        prototipos_base.emplace(elem["id"], UnidadBase(elem,armamento.get(elem["id_arma"])));
        //prototipos.emplace(elem["id"], Unidad(elem,armamento.get(elem["id"])));
    }
}
EjercitoCreador::~EjercitoCreador(){}
Unidad EjercitoCreador::clonar(std::string id_tipo,int id,int x,int y){
    
    return Unidad(id,x,y,prototipos_base.at(id_tipo));
}
}