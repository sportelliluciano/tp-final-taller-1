#include "infraestructura_creador.h"

#include <unordered_map>
#include <string>

#include "edificio.h"
#include "edificio_base.h"

InfraestructuraCreador::InfraestructuraCreador(std::string json_path){
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
        prototipos.emplace(elem["id"], Edificio(info_base[elem["id"]]));
    }
}
InfraestructuraCreador::~InfraestructuraCreador();
Edificio InfraestructuraCreador::clonar(char id_tipo,int x,int y){
    return prototipos[id_tipo].clonar(x,y);
}
