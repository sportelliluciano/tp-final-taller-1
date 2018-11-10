#include "cliente/modelo/gusano_arena.h"

#include "cliente/modelo/sprite.h"
#include "cliente/modelo/terreno.h"
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

void GusanoArena::renderizar(Ventana& ventana) {
    if (mostrar) {
        int x_px, y_px;
        terreno.convertir_a_px(x, y, x_px, y_px);
        sprite.renderizar(ventana, x_px, y_px);
        if (sprite.finalizado())
            mostrar = false;
    }
}


} // namespace cliente
