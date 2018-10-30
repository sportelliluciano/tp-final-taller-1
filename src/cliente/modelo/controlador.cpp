#include "cliente/modelo/controlador.h"

#include "cliente/modelo/juego.h"
#include "cliente/video/ventana.h"

namespace cliente {

Controlador::Controlador(Ventana& ventana_, Juego& juego_) 
    : juego(juego_), ventana(ventana_)
{ 
    ventana.registrar_evento(EVENTO_VENTANA_CERRAR, [this]() {
        juego.detener();
    });
}

void Controlador::actualizar_modelo() {
    
}

} // namespace cliente
