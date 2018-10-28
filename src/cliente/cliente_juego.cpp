#include "cliente/cliente_juego.h"

namespace cliente {

ClienteJuego::ClienteJuego(int argc, char *argv[]) : ventana(800, 600),
    juego(ventana) { }

int ClienteJuego::ejecutar() {
    ventana.registrar_evento(EVENTO_VENTANA_CERRAR, [this]() {
        juego.detener();
    });
    
    while (!juego.esta_terminado()) {
        ventana.procesar_eventos();
        juego.actualizar();
        ventana.actualizar();
    }

    return 0;
}

ClienteJuego::~ClienteJuego() { }

} // namespace cliente
