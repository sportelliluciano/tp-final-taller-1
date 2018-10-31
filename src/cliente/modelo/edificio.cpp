#include "cliente/modelo/edificio.h"

#include "libs/json.hpp"

#include "cliente/modelo/celda.h"
#include "cliente/video/ventana.h"

namespace cliente {

Edificio::Edificio(const nlohmann::json& data_edificio)
{
    id = data_edificio["id"];
    nombre = data_edificio["nombre"];

    Sprite sprite_base = Sprite(data_edificio["sprite_base"]);
    Sprite sprite_construido = Sprite(data_edificio["sprite_construido"]);
    Sprite sprite_destruido = Sprite(data_edificio["sprite_destruido"]);

    sprite = SpriteCompuesto({sprite_construido, sprite_base});
    sprite_roto = SpriteCompuesto({sprite_destruido, sprite_base});

    sprite_construccion = SpriteAnimado(4130, 4136);
    sprite_destruccion  = SpriteAnimado(4137, 4151);

    ancho = data_edificio["dimensiones"][0];
    alto = data_edificio["dimensiones"][1];
    vida = 100;
}

const std::vector<Celda>& Edificio::obtener_celdas_ocupadas() const {
    return celdas_ocupadas;
}

void Edificio::renderizar(const Terreno& terreno, Ventana& ventana) {
    if (!esta_vivo())
        return;

    int x_px, y_px;

    for (auto it=celdas_ocupadas.begin(); it != celdas_ocupadas.end(); ++it) {
        const Celda& celda = *it;
        terreno.convertir_a_px(celda.x(), celda.y(), x_px, y_px);
        Sprite(0, 0, 0)
            .renderizar(ventana, x_px, y_px);
    }

    terreno.convertir_a_px(pos_x, pos_y, x_px, y_px);

    if (!esta_destruido) {
        if (!esta_construido) {
            sprite_construccion.renderizar(ventana, x_px, y_px);
            esta_construido = sprite_construccion.finalizado();
        } else {
            sprite.renderizar(ventana, x_px, y_px);
        }
    } else {
        sprite_destruccion.renderizar(ventana, x_px, y_px);
    }
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

void Edificio::destruir() {
    esta_destruido = true;
}

bool Edificio::esta_vivo() const {
    return !sprite_destruccion.finalizado();
}

} // namespace cliente
