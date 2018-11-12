#include "cliente/modelo/controlador.h"

#include <iostream>

#include "cliente/modelo/hud.h"
#include "cliente/modelo/juego.h"
#include "cliente/eventos/evento.h"
#include "cliente/servidor.h"
#include "cliente/video/ventana.h"
#include "cliente/video/log.h"

namespace cliente {

Controlador::Controlador(Ventana& ventana_, Servidor& servidor_, Juego& juego_) 
    : ventana(ventana_), servidor(servidor_), juego(juego_), 
      hud(ventana_, juego_, servidor_)
{
    ventana.registrar_notificable(hud);
}

void Controlador::procesar_entrada() {
    Evento *evento;
    while (servidor.hay_eventos()) {
        evento = servidor.pop_evento();
        try {
            evento->actualizar(juego);
        } catch (const std::exception& e) {
            log_error("Evento inválido: %s", e.what());
        }
        delete evento;
    }
}

void Controlador::renderizar() {
    hud.renderizar(ventana, 0, 0);
}

} // namespace cliente
