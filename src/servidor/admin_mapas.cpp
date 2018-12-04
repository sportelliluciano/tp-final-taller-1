#include "servidor/admin_mapas.h"

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "libs/json.hpp"

#define ID_JUGADORES "jugadores"

namespace servidor {

static inline const char *basename(const char *archivo) {
    int ultima_barra = 0;
    for (int i=0; archivo[i] != 0; i++) {
        if ((archivo[i] == '/') || (archivo[i] == '\\'))
            ultima_barra = i + 1;
    }

    return archivo + ultima_barra;
}

AdminMapas::AdminMapas(const std::vector<std::string>& rutas_mapas) {
    for (const std::string& mapa : rutas_mapas) {
        std::ifstream archivo_mapa(mapa);

        if (!archivo_mapa.good()) {
            throw std::runtime_error(std::string("No se pudo abrir: ") + mapa);
        }
        
        nlohmann::json data_mapa;
        archivo_mapa >> data_mapa;

        std::string nombre = basename(mapa.c_str());
        mapas.emplace(nombre, data_mapa);
        nombres.push_back(nombre);
    }
}

const std::vector<std::string>& AdminMapas::obtener_nombres() const {
    return nombres;
}

const nlohmann::json& AdminMapas::obtener_mapa(const std::string& nombre) const 
{
    return mapas.at(nombre);
}

bool AdminMapas::existe_mapa(const std::string& nombre) const {
    return mapas.count(nombre) != 0;
}

size_t AdminMapas::obtener_cantidad_jugadores(const std::string& mapa) const {
    const nlohmann::json& data_mapa = mapas.at(mapa);
    return data_mapa.at(ID_JUGADORES)
        .get<std::vector<std::vector<int>>>().size();
}

} // namespace servidor
