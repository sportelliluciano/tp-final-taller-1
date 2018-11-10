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

void Infraestructura::actualizar(int) {
    std::vector<int> edificios_a_eliminar;

    for (auto& par : edificios_construidos) {
        Edificio& edificio = par.second;
        if (!edificio.esta_vivo()) {
            edificios_a_eliminar.push_back(edificio.obtener_id());
        }
    }

    for (int id_a_eliminar: edificios_a_eliminar) {
        edificios_construidos.erase(id_a_eliminar);
    }
    
}

void Infraestructura::construir(int id, const std::string& clase, int x, int y) 
{
    Edificio edificio_nuevo = edificios.at(clase);
    edificio_nuevo.construir(id, x, y);
    edificios_construidos.emplace(id, edificio_nuevo);
    terreno.agregar_edificio(edificios_construidos.at(id));
}

void Infraestructura::destruir(int id) {
    terreno.eliminar_edificio(edificios_construidos.at(id));
    edificios_construidos.at(id).destruir();
}

void Infraestructura::renderizar(Ventana& ventana) {
    for (const Edificio* eid : terreno.obtener_edificios_visibles(ventana)) {
        Edificio& edificio = edificios_construidos.at(eid->obtener_id());

        int x_px, y_px;

        /*** Pintar celda ***/
        int x_celda = edificio.obtener_celda_x(),
            y_celda = edificio.obtener_celda_y();

        for (int x=0; x<edificio.obtener_ancho_celdas();x++) {
            for (int y=0; y<edificio.obtener_alto_celdas();y++) {
                terreno.convertir_a_px(x_celda + x, y_celda + y, x_px, y_px);
                Sprite(0).renderizar(ventana, x_px, y_px);
            }
        }
        /*** Fin pintar celda ***/

        terreno.obtener_posicion_visual(edificio, x_px, y_px);
        edificio.renderizar(ventana, x_px, y_px, 
            &edificio == edificio_seleccionado);
    }
}

void Infraestructura::seleccionar(const Edificio& edificio) {
    if (edificios_construidos.find(edificio.obtener_id()) 
        != edificios_construidos.end())
    {
        edificio_seleccionado = 
            &edificios_construidos.at(edificio.obtener_id());
    }
}

void Infraestructura::limpiar_seleccion() {
    edificio_seleccionado = nullptr;
}

std::vector<const Edificio*> Infraestructura::obtener_edificios() const {
    std::vector<const Edificio*> edificios_disponibles;

    for (auto it=edificios.begin(); it != edificios.end(); ++it) {
        if (it->first == "centro_construccion")
            continue;
        edificios_disponibles.push_back(&it->second);
    }

    return edificios_disponibles;
}

void Infraestructura::iniciar_construccion(const std::string& clase) {
    if (clase == "centro_construccion")
        throw std::runtime_error("Este edificio no debería construirse");
    
    
}

} // namespace cliente
