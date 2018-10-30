#include "cliente/modelo/edificio.h"

#include "libs/json.hpp"

#include "cliente/modelo/celda.h"
#include "cliente/video/ventana.h"

namespace cliente {

Edificio::Edificio(const nlohmann::json& data_edificio)
{
    id = data_edificio["id"];
    nombre = data_edificio["nombre"];
    sprite_base = Sprite(data_edificio["sprite_base"]);
    sprite_construido = Sprite(data_edificio["sprite_construido"]);
    sprite_destruido = Sprite(data_edificio["sprite_destruido"]);
    ancho = data_edificio["dimensiones"][0];
    alto = data_edificio["dimensiones"][1];
    vida = 100;
}

const std::vector<Celda>& Edificio::obtener_celdas_ocupadas() const {
    return celdas_ocupadas;
}

void Edificio::renderizar(const Terreno& terreno, Ventana& ventana) {
    int x_px, y_px;

    terreno.convertir_a_px(pos_x, pos_y, x_px, y_px);

    sprite_base.renderizar(ventana, x_px, y_px);
    if (vida > 30)
        sprite_construido.renderizar(ventana, x_px, y_px);
    else
        sprite_destruido.renderizar(ventana, x_px, y_px);
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
