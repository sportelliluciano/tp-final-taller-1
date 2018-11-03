#include "cliente/modelo/sprite.h"

#define RUTA_SPRITES_NUM "./assets/imgs/imgs/"

namespace cliente {

Sprite::Sprite(int id_, int desplazamiento_x, int desplazamiento_y) {
    id = id_;
    dx = desplazamiento_x;
    dy = desplazamiento_y;
}

Sprite::Sprite(const nlohmann::json& sprite) {
    if (sprite.find("id") != sprite.end())
        id = sprite["id"];
    
    if (sprite.find("x") != sprite.end())
        dx = sprite["x"];
    
    if (sprite.find("y") != sprite.end())
        dy = sprite["y"];
}

void Sprite::desplazar(int desplazamiento_x, int desplazamiento_y) {
    dx = desplazamiento_x;
    dy = desplazamiento_y;
}

void Sprite::renderizar(Ventana& ventana, int x, int y) {
    if (id == -1)
        return;
    
    char nombre[300];
    sprintf(nombre, RUTA_SPRITES_NUM "%05d.bmp", id);

    const Textura& textura = ventana
        .obtener_administrador_texturas()
        .cargar_imagen(nombre);
    
    textura.renderizar(x + dx, y + dy);
}

const Textura& Sprite::obtener_textura(Ventana& ventana) const {
    if (id == -1)
        throw std::runtime_error("El sprite no fue cargado (-1)");
    char nombre[300];
    sprintf(nombre, RUTA_SPRITES_NUM "%05d.bmp", id);

    const Textura& textura = ventana
        .obtener_administrador_texturas()
        .cargar_imagen(nombre);
    
    return textura;
}

} // namespace cliente
