#include "cliente/modelo/hud/boton_construccion.h"

#include <string>

#include "cliente/modelo/hud/boton_temporizado.h"
#include "cliente/modelo/hud/tostador.h"
#include "cliente/modelo/infraestructura.h"
#include "cliente/servidor.h"

namespace cliente {

BotonConstruccion::BotonConstruccion(Infraestructura& infraestructura_, 
    const std::string& clase_, Servidor& servidor_, Tostador& tostador_)
: BotonTemporizado(infraestructura_.obtener_sprite_clase(clase_)),
  infraestructura(infraestructura_),
  clase(clase_),
  servidor(servidor_),
  tostador(tostador_)
{ }

void BotonConstruccion::pre_renderizar(Ventana& ventana, int x, int y) {
    if (infraestructura.esta_construyendo(clase)) {
        set_cola(infraestructura.obtener_cola_construccion(clase));
        set_tiempo(infraestructura.obtener_segundos_restantes(clase));
        if (infraestructura.obtener_segundos_restantes(clase) != 0) {
            set_filtro(true, false);
        } else {
            set_filtro(false, true);
        }
    } else {
        reiniciar();
    }
}

bool BotonConstruccion::mouse_click_izquierdo(int, int) {
    if (infraestructura.esta_construyendo(clase) && 
        infraestructura.obtener_segundos_restantes(clase) == 0)
    {
        if (cb_ubicar_edificio)
            cb_ubicar_edificio();
    } else {
        servidor.iniciar_construccion(clase);
        tostador.hacer_tostada("Iniciar construcción: " + clase);
    }
    return false;
}

bool BotonConstruccion::mouse_click_derecho(int, int) {
    if (infraestructura.esta_construyendo(clase)) {
        servidor.cancelar_construccion(clase);
        tostador.hacer_tostada("Cancelar construcción: " + clase);
    }
    return false;
}

void BotonConstruccion::en_ubicar_nuevo_edificio(
    std::function<void(void)> callback)
{
    cb_ubicar_edificio = callback;
}

} // namespace cliente
