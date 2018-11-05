#include "modelo/infraestructura_creador.h"

#include <unordered_map>
#include <string>

#include "modelo/edificio.h"
#include "modelo/edificio_base.h"

namespace modelo {

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
unsigned int InfraestructuraCreador::get_costo(id_tipo){
    return info_base[id_tipo].get_costo();
}
}
