#include "cliente/modelo/disparo_plasma.h"

#include "cliente/modelo/disparo_proyectil.h"
#include "cliente/modelo/sprite_animado.h"
#include "cliente/video/camara.h"
#include "cliente/video/posicion.h"
#include "cliente/video/ventana.h"

namespace cliente {

DisparoPlasma::DisparoPlasma() 
: DisparoProyectil(1.0f)
{
    plasma = SpriteAnimado(3496, 3496 + 31);
    plasma.configurar_repeticion(true);
    plasma.set_centrado(true);
    explosion = SpriteAnimado(3939, 3939 + 34);
    explosion.set_centrado(true);
}

void DisparoPlasma::renderizar(Ventana& ventana, Camara& camara) {
    if (!esta_activo && !esta_viajando && !esta_explotando)
        return;
    
    Posicion visual = camara.traducir_a_visual(pos_actual);

    ventana.cambiar_plano(true);

    if (esta_viajando) {
        // mostrar en el aire    
        plasma.renderizar(ventana, visual.x, visual.y);
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
