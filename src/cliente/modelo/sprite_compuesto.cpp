#include "cliente/modelo/sprite_compuesto.h"

#include <vector>

#include "cliente/modelo/sprite.h"
#include "cliente/video/ventana.h"

namespace cliente {

SpriteCompuesto::SpriteCompuesto(const std::vector<Sprite>& sprites_) {
    sprites = sprites_;
}

void SpriteCompuesto::agregar_sprite(const Sprite& sprite) {
    sprites.push_back(sprite);
}

void SpriteCompuesto::agregar_sprite(int id_sprite, int dx, int dy) {
    sprites.push_back(Sprite(id_sprite, dx, dy));
}

void SpriteCompuesto::desplazar_subsprite(int id, int dx, int dy) {
    if ((id < 0) || (id >= sprites.size()))
        throw std::runtime_error("SpriteCompuesto: Cuadro inexistente");
    
    sprites[id].desplazar(dx, dy);
}

void SpriteCompuesto::renderizar(Ventana& ventana, int x, int y) {
    for (auto it = sprites.begin(); it != sprites.end(); ++it) {
        Sprite& sprite = *it;
        sprite.renderizar(ventana, x, y);
    }
}

} // namespace cliente
