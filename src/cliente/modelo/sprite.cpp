#include "cliente/modelo/sprite.h"

#include "cliente/config.h"

namespace cliente {

/**
 * \brief Genera el nombre de la textura y la carga desde el administrador
 *        de texturas de la ventana.
 */
static const Textura& cargar_textura(Ventana& ventana, int id,
    const std::string& ruta) 
{
    if (id == -1)
        return ventana.obtener_administrador_texturas().cargar_imagen(ruta);
    
    if (id > 999999)
        throw std::runtime_error("ID de sprite inválido");
    
    char nombre[sizeof(RUTA_SPRITES) + sizeof("/000000.bmp")];
    sprintf(nombre, RUTA_SPRITES "/%05d.bmp", id);

    const Textura& textura = ventana
        .obtener_administrador_texturas()
        .cargar_imagen(nombre);
    
    return textura;
}

Sprite::Sprite(int id_, int desplazamiento_x, int desplazamiento_y) {
    id = id_;
    dx = desplazamiento_x;
    dy = desplazamiento_y;
}

Sprite Sprite::desde_json(const nlohmann::json& sprite) {
    int id = -1, dx = 0, dy = 0;
    if (sprite.find("id") != sprite.end())
        id = sprite["id"];
    
    if (sprite.find("x") != sprite.end())
        dx = sprite["x"];
    
    if (sprite.find("y") != sprite.end())
        dy = sprite["y"];
    
    return Sprite(id, dx, dy);
}

Sprite::Sprite(const std::string& ruta_) {
    ruta = ruta_;
}

void Sprite::desplazar(int desplazamiento_x, int desplazamiento_y) {
    dx = desplazamiento_x;
    dy = desplazamiento_y;
}

void Sprite::renderizar(Ventana& ventana, int x, int y) {
    if (id == -1) {
        const Textura& textura = ventana
            .obtener_administrador_texturas()
            .cargar_imagen(ruta);
        textura.renderizar(x + dx, y + dy);
        return;
    }

    if (id > 999999)
        throw std::runtime_error("ID de sprite inválido");
    
    char nombre[sizeof(RUTA_SPRITES) + sizeof("/000000.bmp")];
    sprintf(nombre, RUTA_SPRITES "/%05d.bmp", id);

    const Textura& textura = ventana
        .obtener_administrador_texturas()
        .cargar_imagen(nombre);
    
    textura.renderizar(x + dx, y + dy);
}

int Sprite::obtener_alto(Ventana& ventana) const {
    if (id == -1)
        return 0;
    return cargar_textura(ventana, id, ruta).obtener_alto();
}

int Sprite::obtener_ancho(Ventana& ventana) const {
    if (id == -1)
        return 0;
    return cargar_textura(ventana, id, ruta).obtener_ancho();
}

int Sprite::obtener_desplazamiento_x() const {
    return dx;
}

int Sprite::obtener_desplazamiento_y() const {
    return dy;
}


int Sprite::obtener_id() const {
    return id;
}

const Textura& Sprite::obtener_textura(Ventana& ventana) {
    return cargar_textura(ventana, id, ruta);
}

} // namespace cliente
