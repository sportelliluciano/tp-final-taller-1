#include "cliente/cliente_juego.h"

namespace cliente {

ClienteJuego::ClienteJuego(int argc, char *argv[]) 
: ventana(800, 600), juego(argv[1]), controlador(ventana, juego)
{ }

int ClienteJuego::ejecutar() {
    while (!juego.esta_terminado()) {
        ventana.procesar_eventos();
        controlador.actualizar_modelo();
        juego.renderizar(ventana);
        ventana.actualizar();
    }

    return 0;
}

ClienteJuego::~ClienteJuego() { }

} // namespace cliente
