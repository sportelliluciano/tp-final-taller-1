#include "modelo/arma_creador.h"

#include <fstream>

#include "libs/json.hpp"
#include "modelo/arma_base.h"
#include "modelo/arma_caracteristicas.h"

namespace modelo {

ArmaCreador::ArmaCreador(){

    armas_base_iniziaizador();
    //armas_caracteristicas();

    using nlohmann::json;

    std::ifstream entrada("../data/armas.json");

    json edificios_json;

    entrada >> edificios_json;

    auto it = edificios_json.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for(; it != edificios_json.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);
        prototipos.emplace(elem["id"], Arma(&armas_base[elem["id_base"]],
                                    &armas_caracteristicas[elem["id_bono"]]));
    }
}
Arma* ArmaCreador::get(int id){
    return &prototipos[id];
}
void ArmaCreador::armas_base_iniziaizador(){
    using nlohmann::json;

    std::ifstream entrada("../data/armas_base.json");

    json edificios_json;

    entrada >> edificios_json;

    auto it = edificios_json.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for(; it != edificios_json.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);
        armas_base.emplace(elem["id"], ArmaBase(elem));
    }
}
}

/*
void ArmaCreador::armas_caracteristicas(){
    using nlohmann::json;

    std::ifstream entrada("../data/armas_bono.json");

    json edificios_json;

    entrada >> edificios_json;

    auto it = edificios_json.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for(; it != edificios_json.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);
        armas_caracteristicas.emplace(elem["id"], ArmaCaracteristicas(elem));
    }
}
*/
