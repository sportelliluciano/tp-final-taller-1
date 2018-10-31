#include "cliente/cliente_juego.h"

#include "cliente/modelo/controlador.h"
#include "cliente/modelo/juego.h"
#include "cliente/video/ventana.h"
#include <iostream>
namespace cliente {

ClienteJuego::ClienteJuego(Servidor& servidor_, int argc, char *argv[]) 
    : ventana(1024, 600), servidor(servidor_), juego(argv[1]), 
      controlador(ventana, servidor, juego)
{ }

int ClienteJuego::ejecutar() {
    while (!juego.esta_terminado()) {
        ventana.procesar_eventos();
        juego.renderizar(ventana);
        controlador.actualizar_modelo();
        controlador.renderizar();
        ventana.actualizar();
    }
    
    return 0;
}

ClienteJuego::~ClienteJuego() { }

} // namespace cliente
