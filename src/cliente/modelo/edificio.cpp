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
    
    sprite_boton = data_edificio["sprite_boton"];

    ancho = data_edificio["dimensiones"][0];
    alto = data_edificio["dimensiones"][1];
    vida = 100;
}

void Edificio::renderizar(Ventana& ventana, int x_px, int y_px, bool selecc) {
    if (!esta_vivo())
        return;

    if (!esta_destruido) {
        if (!esta_construido) {
            sprite_construccion.renderizar(ventana, x_px, y_px);
            esta_construido = sprite_construccion.finalizado();
        } else {
            sprite.renderizar(ventana, x_px, y_px);
            if (selecc) {
                ventana
                    .obtener_administrador_texturas()
                    .cargar_imagen("./assets/nuevos/seleccion-edificio.png")
                    .renderizar(x_px + sprite.obtener_ancho(ventana) / 2, y_px);
            }
        }
    } else {
        sprite_destruccion.renderizar(ventana, x_px, y_px);
    }
}

void Edificio::construir(int id_edificio_, int x, int y) {
    pos_x = x;
    pos_y = y;
    id_edificio = id_edificio_;
}

int Edificio::obtener_id() const {
    return id_edificio;
}

const std::string& Edificio::obtener_clase() const {
    return id;
}

int Edificio::obtener_sprite_boton() const {
    return sprite_boton;
}

void Edificio::destruir() {
    esta_destruido = true;
}

int Edificio::obtener_celda_x() const {
    return pos_x;
}

int Edificio::obtener_celda_y() const {
    return pos_y;
}

int Edificio::obtener_ancho_celdas() const {
    return ancho;
}

int Edificio::obtener_alto_celdas() const {
    return alto;
}

bool Edificio::esta_vivo() const {
    return !sprite_destruccion.finalizado();
}

} // namespace cliente
