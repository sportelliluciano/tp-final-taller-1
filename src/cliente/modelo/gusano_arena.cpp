#include "cliente/modelo/gusano_arena.h"

#include "cliente/modelo/sprite.h"
#include "cliente/modelo/terreno.h"
#include "cliente/video/camara.h"
#include "cliente/video/ventana.h"

#define SPRITE_INICIAL 3802
#define SPRITE_FINAL 3817

namespace cliente {

GusanoArena::GusanoArena(Terreno& terreno_) 
    : terreno(terreno_), sprite(SPRITE_INICIAL, SPRITE_FINAL, 10) 
{ }

void GusanoArena::aparecer(int x_, int y_) {
    x = x_; y = y_;
    sprite.reiniciar();
    mostrar = true;
}

void GusanoArena::renderizar(Ventana& ventana, Camara& camara) {
    if (mostrar) {
        // TODO: no renderizar si no es visible
        Posicion visual = camara.traducir_a_visual(Posicion(x, y));
        sprite.renderizar(ventana, visual.x, visual.y);
        if (sprite.finalizado())
            mostrar = false;
    }
}


} // namespace cliente
