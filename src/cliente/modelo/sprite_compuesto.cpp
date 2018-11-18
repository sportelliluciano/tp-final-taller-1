#include "cliente/modelo/sprite_compuesto.h"

#include <vector>

#include "cliente/modelo/sprite.h"
#include "cliente/video/ventana.h"

namespace cliente {

SpriteCompuesto::SpriteCompuesto(const std::vector<Sprite>& sprites_) {
    for (const Sprite& s : sprites_) {
        if (s.obtener_id() != -1)
            sprites.push_back(s);
    }
    for (const Sprite& s : sprites) {
        s_nombre << s.obtener_id() << "-";
    }
}

SpriteCompuesto::SpriteCompuesto(const SpriteCompuesto& otro) {
    sprites = otro.sprites;
    s_nombre.str(otro.s_nombre.str());
}

SpriteCompuesto& SpriteCompuesto::operator=(const SpriteCompuesto& otro) {
    sprites = otro.sprites;
    s_nombre.str(otro.s_nombre.str());
}

void SpriteCompuesto::agregar_sprite(const Sprite& sprite) {
    sprites.push_back(sprite);
    s_nombre << sprite.obtener_id() << "-";
}

void SpriteCompuesto::agregar_sprite(int id_sprite, int dx, int dy) {
    agregar_sprite(Sprite(id_sprite, dx, dy));
}

void SpriteCompuesto::desplazar_subsprite(int id, int dx, int dy) {
    if ((id < 0) || (id >= sprites.size()))
        throw std::runtime_error("SpriteCompuesto: Cuadro inexistente");
    
    sprites[id].desplazar(dx, dy);
    invalidar = true;
}

void SpriteCompuesto::renderizar(Ventana& ventana, int x, int y) {
    obtener_textura(ventana).renderizar(x, y);
}

static Textura& obtener_o_crear_textura(
    Ventana& ventana, const std::string& nombre, int ancho, int alto) 
{
    AdministradorTexturas& admin_texturas
        = ventana.obtener_administrador_texturas();
    
    if (admin_texturas.contiene_textura(nombre))
        return admin_texturas.obtener_textura(nombre);
    
    return admin_texturas.crear_textura(nombre, ancho, alto);
}

const Textura& SpriteCompuesto::obtener_textura(Ventana& ventana) {
    int ancho = 0, alto = 0, min_despl_x = 0, min_despl_y = 0;
    for (const Sprite& s : sprites) {
        int full_ancho = s.obtener_ancho(ventana) 
            + abs(s.obtener_desplazamiento_x());
        int full_alto = s.obtener_alto(ventana)
            + abs(s.obtener_desplazamiento_y());
        
        if (min_despl_x > s.obtener_desplazamiento_x())
            min_despl_x = s.obtener_desplazamiento_x();
        
        if (min_despl_y > s.obtener_desplazamiento_y())
            min_despl_y = s.obtener_desplazamiento_y();

        if (full_ancho > ancho)
            ancho = full_ancho;
        if (full_alto > alto)
            alto = full_alto;
    }
    
    Textura& textura = obtener_o_crear_textura(
        ventana, s_nombre.str(), ancho, alto);
    
    if (invalidar) {
        invalidar = false;
        textura.limpiar();

        for (auto it = sprites.begin(); it != sprites.end(); ++it) {
            Sprite& sprite = *it;
            sprite
                .obtener_textura(ventana)
                .renderizar(sprite.obtener_desplazamiento_x() - min_despl_x, 
                    sprite.obtener_desplazamiento_y() - min_despl_y, textura);
        }
    }

    return textura;
}

} // namespace cliente
