#include "cliente/modelo/tropa.h"

#include "cliente/video/ventana.h"

#define SPRITE_BASE 206

namespace cliente {

Tropa::Tropa() {
    for (int i=0;i<N_SPRITES;i++) {
        sprites[i] = SpriteAnimado({SPRITE_BASE + i, 
            SPRITE_BASE + N_SPRITES + i}, 5);
        
        sprites[i].configurar_repeticion(true);
    }

    x = y = 100;
    x_destino = 200;
    y_destino = 100;
}

void Tropa::renderizar(Ventana& ventana) {
    if ((x_destino == x) && (y_destino == y)) {
        sprites[0].renderizar(ventana, (int)x, (int)y);
        return;
    }

    sprites[10].renderizar(ventana, (int)x, (int)y);

    float vx = 0, vy = 0;

    if (x_destino != x) {
        vx = (x_destino - x) / abs(x_destino - x);
        vx *= 0.4;
    }

    if (y_destino != y) {
        vy = (y_destino - y) / abs(y_destino - y);
        vy *= 0.4;
    }

    if (abs(x + vy) > abs(x_destino))
        x = x_destino;
    else
        x += vx;
    
    if (abs(y + vy) > abs(y_destino))
        y = y_destino;
    else
        y += vy;
}

} // namespace cliente