#include "cliente/modelo/tropa.h"

#include "cliente/video/log.h"
#include "cliente/video/ventana.h"

#define SPRITE_BASE 206
#define THRESHOLD_SYNC_CAMINO 10

namespace cliente {

Tropa::Tropa(int id_tropa_, int x_, int y_) {
    for (int i=0;i<N_SPRITES;i++) {
        sprites[i] = SpriteAnimado({SPRITE_BASE + i, 
            SPRITE_BASE + N_SPRITES + i}, 5);
        
        sprites[i].configurar_repeticion(true);
    }

    x_destino = x = x_;
    y_destino = y = y_;
    id_tropa = id_tropa_;
}

void Tropa::renderizar(Ventana& ventana, int x_px, int y_px) {
    if ((x_destino == x) && (y_destino == y)) {
        sprites[0].renderizar(ventana, x_px, y_px);
        return;
    }

    sprites[10].renderizar(ventana, x_px, y_px);
}

void Tropa::actualizar(int dt_ms) {
    float vx = 0, vy = 0;

    if (x_destino != x) {
        vx = (x_destino - x) / abs(x_destino - x);
        vx *= 0.4 / 15;
    }

    if (y_destino != y) {
        vy = (y_destino - y) / abs(y_destino - y);
        vy *= 0.4 / 15;
    }

    float dx = vx * dt_ms,
          dy = vy * dt_ms;

    if (abs(x + dx) > abs(x_destino))
        x = x_destino;
    else
        x += dx;
    
    if (abs(y + dy) > abs(y_destino))
        y = y_destino;
    else
        y += dy;
}

int Tropa::obtener_id() const {
    return id_tropa;
}

int Tropa::obtener_x() const {
    return x;
}

int Tropa::obtener_y() const {
    return y;
}

bool Tropa::esta_moviendo() const {
    return (x_destino != x) || (y_destino != y);
}

void Tropa::caminar_hacia(int x_dest, int y_dest) {
    x_destino = x_dest;
    y_destino = y_dest;

    // TODO
    log_depuracion("Caminando hacia (%.2f, %.2f)", x_destino, y_destino);
}

void Tropa::seguir_camino(const std::vector<std::pair<int, int>>& camino) {
    camino_actual = camino;
    paso_actual = 0;
    
    // Iniciar la caminata.
    sync_camino(camino_actual[paso_actual].first, 
        camino_actual[paso_actual].second);
}

void Tropa::sync_camino(int x_, int y_) {
    /*** Chequear posiciones -- dbg ***/
    float mse = ((x - x_) * (x - x_)) + (y - y_) * (y - y_);
    if (mse > THRESHOLD_SYNC_CAMINO) {
        log_advertencia("El MSE entre posiciones es >%d [%.2f]", mse,
            THRESHOLD_SYNC_CAMINO);
    }
    
    /******* TODO: Eliminar esto ******/
    log_depuracion("(%.1f, %.1f) <> (%d, %d)", x, y, x_, y_);
    x = x_;
    y = y_;

    paso_actual++;
    if (paso_actual < camino_actual.size()) {
        caminar_hacia(camino_actual[paso_actual].first, 
            camino_actual[paso_actual].second);
    }
}

} // namespace cliente