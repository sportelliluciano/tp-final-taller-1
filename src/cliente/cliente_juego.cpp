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
        // Procesar eventos
        ventana.procesar_eventos(); // Cerrar ventana
        controlador.procesar_entrada(); // Mouse / teclado
        
        // Actualizar el modelo del juego
        controlador.actualizar_modelo();
        
        // Renderizar el juego
        juego.renderizar(ventana);
        controlador.renderizar(); // Renderizar el HUD

        // Mostrar los cambios
        ventana.actualizar();
    }
    
    return 0;
}

ClienteJuego::~ClienteJuego() { }

} // namespace cliente
