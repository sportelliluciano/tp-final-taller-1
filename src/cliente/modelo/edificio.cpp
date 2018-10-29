#include "cliente/modelo/edificio.h"

#include <cstdio>

#include "cliente/json.hpp"

#include "cliente/modelo/celda.h"
#include "cliente/video/ventana.h"

namespace cliente {

Edificio::Edificio(const nlohmann::json& data_edificio)
{
    id = data_edificio["id"];
    nombre = data_edificio["nombre"];
    sprite_base = data_edificio["sprite_base"];
    sprite_construido = data_edificio["sprite_construido"];
    sprite_destruido = data_edificio["sprite_destruido"];
    ancho = data_edificio["dimensiones"][0];
    alto = data_edificio["dimensiones"][1];
    desp_base_x = data_edificio["desplazamiento_x"];
    desp_base_y = data_edificio["desplazamiento_y"];
    vida = 100;
}

const std::vector<Celda>& Edificio::obtener_celdas_ocupadas() const {
    return celdas_ocupadas;
}

void Edificio::renderizar(const Terreno& terreno, Ventana& ventana) {
    int x_px, y_px;

    terreno.convertir_a_px(pos_x, pos_y, x_px, y_px);

    char nombre_sprite_base[300];
    int y_base = 0;

    if (sprite_base != -1) {
        sprintf(nombre_sprite_base, "./assets/imgs/imgs-numeradas/%05d.bmp", 
            sprite_base);
        const Textura& textura = ventana
            .obtener_administrador_texturas()
            .cargar_imagen(nombre_sprite_base);
        
        textura.renderizar(x_px, y_px);
    }

    sprintf(nombre_sprite_base, "./assets/imgs/imgs-numeradas/%05d.bmp", 
        sprite_construido);
    const Textura& textura_construido = ventana
        .obtener_administrador_texturas()
        .cargar_imagen(nombre_sprite_base);
    
    textura_construido.renderizar(x_px + desp_base_x, y_px + desp_base_y);
}

void Edificio::construir(const Terreno& terreno, int x, int y) {
    if (celdas_ocupadas.size() != 0)
        throw std::runtime_error("Se quizo construir un edificio ya construido");
    
    pos_x = x;
    pos_y = y;

    for (int i=0; i < ancho; i++) {
        for (int j=0; j < alto; j++) {
            celdas_ocupadas.push_back(terreno.at(x + i, y + j));
        }
    }
}

} // namespace cliente