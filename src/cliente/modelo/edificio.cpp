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

    sprite_construccion = SpriteAnimado(3765, 3783);
    sprite_destruccion  = SpriteAnimado(3686, 3700);
    
    sprite_boton = data_edificio["sprite_boton"];

    ancho = data_edificio["dimensiones"][0];
    alto = data_edificio["dimensiones"][1];
    vida = 100;
}

void Edificio::renderizar(Ventana& ventana, int x_px, int y_px) {
    if (!esta_vivo())
        return;

    if (!esta_destruido) {
        if (!esta_construido) {
            sprite_construccion.renderizar(ventana, x_px, y_px);
            esta_construido = sprite_construccion.finalizado();
        } else {
            sprite.renderizar(ventana, x_px, y_px);
            if (marcado) {
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

void Edificio::inicializar(int id_, int x, int y, bool construido) {
    id_edificio = id_;
    pos_x = x;
    pos_y = y;
    esta_construido = construido;
    esta_destruido = false;
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

void Edificio::set_vida(int nueva_vida) {
    vida = nueva_vida;
}

void Edificio::marcar() {
    marcado = true;
}

void Edificio::desmarcar() {
    marcado = false;
}

} // namespace cliente
