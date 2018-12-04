#include "modelo/arma_creador.h"

#include <iostream>
#include <fstream>
#include <string>

#include "libs/json.hpp"
#include "modelo/arma_base.h"
//#include "modelo/arma_caracteristicas.h"

namespace modelo {

ArmaCreador::ArmaCreador() { }

void ArmaCreador::inicializar(const nlohmann::json& data_armas_base) {
    inicializar_armas_base(data_armas_base);
    //armas_caracteristicas();

    using nlohmann::json;

    auto it = data_armas_base.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for(; it != data_armas_base.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);
        prototipos.emplace(elem["id"], Arma(armas_base.at(elem["id"])));
    }
}
ArmaCreador::~ArmaCreador(){
}
Arma& ArmaCreador::get(std::string id){
    return prototipos.at(id);
}
void ArmaCreador::inicializar_armas_base(const nlohmann::json& data_armas_base) 
{
    using nlohmann::json;

    auto it = data_armas_base.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for(; it != data_armas_base.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);
        armas_base.emplace(elem["id"], ArmaBase(elem));
    }
}
bool ArmaCreador::tiene(std::string id){
    return prototipos.count(id)>0;
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
