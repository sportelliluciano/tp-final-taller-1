#include "cliente/modelo/hud/boton_construccion.h"

#include <string>
#include <sstream>

#include "cliente/modelo/hud/boton_temporizado.h"
#include "cliente/modelo/hud/tostador.h"
#include "cliente/modelo/infraestructura.h"
#include "cliente/servidor.h"
#include "cliente/sonido/sonido.h"

namespace cliente {

BotonConstruccion::BotonConstruccion(Infraestructura& infraestructura_, 
    const std::string& clase_, Servidor& servidor_, Tostador& tostador_)
: BotonTemporizado(infraestructura_.obtener_sprite_clase(clase_)),
  infraestructura(infraestructura_),
  clase(clase_),
  servidor(servidor_),
  tostador(tostador_)
{
    const Edificio& base = infraestructura.obtener_edificio_base(clase);
    tooltip.set_titulo(base.obtener_nombre());
    tooltip.set_cuerpo(base.obtener_descripcion());
    tooltip.set_metadata(base.obtener_metadata());
    tooltip.set_costo_tiempo(base.obtener_costo(), 
        base.obtener_tiempo_construccion());
}

void BotonConstruccion::renderizar(Ventana& ventana, const Posicion& punto) {
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
    BotonTemporizado::renderizar(ventana, punto);
}

bool BotonConstruccion::mouse_click_izquierdo(const Posicion&) {
    if (infraestructura.esta_construyendo(clase) && 
        infraestructura.obtener_segundos_restantes(clase) == 0)
    {
        if (cb_ubicar_edificio)
            cb_ubicar_edificio();
    } else {
        Sonido::obtener_instancia().reproducir_sonido(SONIDO_BLEEP);
        servidor.iniciar_construccion(clase);
        tostador.hacer_tostada("Iniciar construcción: " + clase);
    }
    return false;
}

bool BotonConstruccion::mouse_click_derecho(const Posicion&) {
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
