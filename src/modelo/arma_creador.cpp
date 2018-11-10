#include "modelo/arma_creador.h"

#include <iostream>
#include <fstream>
#include <string>

#include "libs/json.hpp"
#include "modelo/arma_base.h"
//#include "modelo/arma_caracteristicas.h"

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
        /*prototipos.emplace(elem["id"], Arma(armas_base.at(elem["id_base"]),
                                    armas_caracteristicas.at(elem["id_bono"])));*/
        std::cout << "arma creada: " << elem["id"] << '\n';
        std::cout << "con arma base: " << elem["id_base"] << '\n';
        prototipos.emplace(elem["id"], Arma(armas_base.at(elem["id_base"])));
    }
}
ArmaCreador::~ArmaCreador(){
}
Arma& ArmaCreador::get(std::string id){
    return prototipos.at(id);
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
        std::cout << "arma base creada: " << elem["id"] << '\n'; 
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
