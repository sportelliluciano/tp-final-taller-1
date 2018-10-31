#include "cliente/modelo/hud/boton.h"

#include "cliente/modelo/sprite.h"
#include "cliente/video/ventana.h"

namespace cliente {

Boton::Boton(int sprite_id, int x_, int y_)
    : sprite(sprite_id), x(x_), y(y_) 
{ }

void Boton::renderizar(Ventana& ventana) {
    Sprite(sprite, 0, 0).renderizar(ventana, x, y);
}

} // namespace cliente
