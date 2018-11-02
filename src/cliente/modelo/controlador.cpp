#include "cliente/modelo/controlador.h"

#include "cliente/modelo/hud.h"
#include "cliente/modelo/juego.h"
#include "conexion/evento.h"
#include "cliente/servidor.h"
#include "cliente/video/ventana.h"

namespace cliente {

Controlador::Controlador(Ventana& ventana_, Servidor& servidor_, Juego& juego_) 
    : ventana(ventana_), servidor(servidor_), juego(juego_)
{ 
    ventana.registrar_evento(EVENTO_VENTANA_CERRAR, [this]() {
        juego.detener();
    });
}

void Controlador::actualizar_modelo() {
    conexion::Evento *evento;
    while (servidor.hay_eventos()) {
        evento = servidor.pop_evento();
        evento->actualizar(juego);
        delete evento;
    }
}

void Controlador::renderizar() {
    hud.renderizar(ventana, juego);
}

} // namespace cliente
