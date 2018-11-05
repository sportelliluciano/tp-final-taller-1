#include "ejercito_creador.h"

#include <unordered_map>

#include "libs/json.hpp"
#include "unidad.h"

EjercitoCreador(Terreno& terreno):terreno(terreno){
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
        prototipos_base.emplace(elem["id"], UnidadBase(elem,armamento.get(elem["id_arma"])));
        //prototipos.emplace(elem["id"], Unidad(elem,armamento.get(elem["id"])));
    }
}
~EjercitoCreador(){}
Unidad clonar(char id_tipo,int x,int y){
    return Unidad(x,y,prototipos_base[id_tipo]);
}
