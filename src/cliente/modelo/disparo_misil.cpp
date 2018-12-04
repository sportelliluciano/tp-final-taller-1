#include "cliente/modelo/disparo_misil.h"

#include "cliente/modelo/disparo_proyectil.h"
#include "cliente/modelo/sprite_animado.h"
#include "cliente/video/camara.h"
#include "cliente/video/posicion.h"
#include "cliente/video/ventana.h"

namespace cliente {

DisparoMisil::DisparoMisil() 
: DisparoProyectil(1.0f)
{
    orientacion_actual = 0;
    explosion = SpriteAnimado(3634, 3638);
    explosion.set_centrado(true);
    for (int i=0;i<32;i++) {
        std::vector<Sprite> cuadros;
        for (int j=0;j<5;j++) {
            cuadros.push_back(Sprite(3336 + 32*j + i));
        }
        misil[i] = SpriteAnimado(cuadros);
        misil[i].configurar_repeticion(true);
        misil[i].set_centrado(true);
    }
}

void DisparoMisil::actualizar(int dt_ms) {
    int max_orientacion = 32;
    if (orientacion != orientacion_actual) {   
        int delta = orientacion - orientacion_actual;
        if (abs(delta) > max_orientacion - abs(delta))
            delta = -delta;

        delta = delta / abs(delta);

        orientacion_actual = orientacion_actual + delta;
        if (orientacion_actual < 0)
            orientacion_actual = max_orientacion - 1;
        else if (orientacion_actual > max_orientacion - 1)
            orientacion_actual = 0;
    }

    DisparoProyectil::actualizar(dt_ms);
}

void DisparoMisil::renderizar(Ventana& ventana, Camara& camara) {
    if (!esta_activo && !esta_viajando && !esta_explotando)
        return;
    
    ventana.cambiar_plano(true);
    Posicion visual = camara.traducir_a_visual(pos_actual);

    if (esta_viajando) {
        // mostrar en el aire
        misil[orientacion].renderizar(ventana, visual.x, visual.y);
    } else if (esta_explotando) {
        explosion.renderizar(ventana, visual.x, visual.y);
        if (explosion.finalizado()) {
            esta_explotando = false;
            explosion.reiniciar();
        }
    }
    ventana.cambiar_plano(false);
}

} // namespace cliente
