#include "cliente/modelo/hud/boton_entrenamiento.h"

#include <string>

#include "cliente/modelo/hud/boton_temporizado.h"
#include "cliente/modelo/ejercito.h"
#include "cliente/modelo/hud/tostador.h"
#include "cliente/servidor.h"
#include "cliente/sonido/sonido.h"

namespace cliente {

BotonEntrenamiento::BotonEntrenamiento(Ejercito& ejercito_, 
    const std::string& clase_, Servidor& servidor_, Tostador& tostador_)
: BotonTemporizado(ejercito_.obtener_sprite_clase(clase_)),
  clase(clase_),
  ejercito(ejercito_),
  servidor(servidor_),
  tostador(tostador_)
{ }

void BotonEntrenamiento::pre_renderizar(Ventana& ventana, int x, int y) {
    if (!ejercito.esta_habilitada(clase)) {
        reiniciar();
        set_filtro(true, false);
    } else if (ejercito.esta_entrenando(clase)) {
        set_cola(ejercito.obtener_cola_entrenamiento(clase));
        set_tiempo(ejercito.obtener_segundos_restantes(clase));
        if (ejercito.obtener_segundos_restantes(clase) != 0) {
            set_filtro(true, false);
        } else {
            set_filtro(false, true);
        }
    } else {
        reiniciar();
    }
}

bool BotonEntrenamiento::mouse_click_izquierdo(int, int) {
    if (!ejercito.esta_habilitada(clase))
        return false;
    
    Sonido::obtener_instancia().reproducir_sonido(SONIDO_BLEEP);
    servidor.iniciar_entrenamiento(clase);
    tostador.hacer_tostada("Iniciar entrenamiento: " + clase);
    return false;
}

bool BotonEntrenamiento::mouse_click_derecho(int, int) {
    if (!ejercito.esta_habilitada(clase))
        return false;
    if (ejercito.esta_entrenando(clase)) {
        servidor.cancelar_entrenamiento(clase);
        tostador.hacer_tostada("Cancelar entrenamiento: " + clase);
    }
    return false;
}

} // namespace cliente
