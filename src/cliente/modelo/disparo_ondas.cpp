#include "cliente/modelo/disparo_ondas.h"

#include "cliente/modelo/disparo.h"
#include "cliente/modelo/sprite_animado.h"
#include "cliente/video/camara.h"
#include "cliente/video/posicion.h"
#include "cliente/video/ventana.h"

namespace cliente {

DisparoOndas::DisparoOndas() 
: DisparoProyectil(1.0f)
{
    explosion = SpriteAnimado(3634, 3638);
    explosion.set_centrado(true);
    ondas = SpriteAnimado(3884, 3884 + 14);
    ondas.set_centrado(true);
    ondas.configurar_repeticion(true);
}

void DisparoOndas::actualizar_destino(int x_destino, int y_destino) {
    nueva_pos_victima.x = x_destino;
    nueva_pos_victima.y = y_destino;
}

void DisparoOndas::actualizar(int dt_ms) {
    if (esta_explotando)
        return;
    
    if (esta_activo && !esta_viajando) {
        // Crear nuevo misil
        pos_actual = origen;
        fx_actual = origen.x;
        fy_actual = origen.y;
        pos_victima = nueva_pos_victima;
        esta_viajando = true;
    } else if (esta_viajando) {
        // Interpolar la posición
        actualizar_movimiento(dt_ms);
        if ((pos_actual == pos_victima) || (tiempo_para_impacto > periodo)) {
            esta_viajando = false;
            esta_explotando = true;
            tiempo_para_impacto = 0;
        }
        tiempo_para_impacto += dt_ms;
    }
}

void DisparoOndas::renderizar(Ventana& ventana, Camara& camara) {
    if (!esta_activo && !esta_viajando && !esta_explotando)
        return;
    
    Posicion visual = camara.traducir_a_visual(pos_actual);

    if (esta_viajando) {
        ondas.renderizar(ventana, visual.x, visual.y);
    } else if (esta_explotando) {
        ventana.cambiar_plano(true);
        explosion.renderizar(ventana, visual.x, visual.y);
        if (explosion.finalizado()) {
            esta_explotando = false;
            explosion.reiniciar();
        }
        ventana.cambiar_plano(false);
    }
}

} // namespace cliente
