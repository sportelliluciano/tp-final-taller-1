#include "cliente/modelo/infraestructura.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "libs/json.hpp"

#include "cliente/modelo/terreno.h"

namespace cliente {

Infraestructura::Infraestructura(Terreno& terreno_juego) 
: terreno(terreno_juego)
{ 
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

        edificios.emplace(elem["id"], Edificio(elem));
    }
}

void Infraestructura::construir(int id, const std::string& clase, int x, int y) 
{
    Edificio edificio_nuevo = edificios.at(clase);
    edificio_nuevo.construir(terreno, x, y);
    edificios_construidos.emplace(id, edificio_nuevo);
}

void Infraestructura::destruir(int id) {
    edificios_construidos.at(id).destruir();
}

void Infraestructura::renderizar(Ventana& ventana) {
    std::vector<int> edificios_a_eliminar;

    for (auto it = edificios_construidos.begin(); 
              it != edificios_construidos.end(); ++it) {
        Edificio& edificio = it->second;
        
        if (!edificio.esta_vivo()) {
            edificios_a_eliminar.push_back(it->first);
            continue;
        }

        if (terreno.esta_en_camara(edificio.obtener_celdas_ocupadas(), 
            ventana)) {
            edificio.renderizar(terreno, ventana);
        }
    }

    for (auto it=edificios_a_eliminar.begin(); it != edificios_a_eliminar.end();
              ++it) 
    {
        edificios_construidos.erase(*it);
    }
}

} // namespace cliente
