#include "cliente/cliente_juego.h"

#include "cliente/modelo/controlador.h"
#include "cliente/modelo/juego.h"
#include "cliente/servidor.h"
#include "cliente/video/ventana.h"

namespace cliente {

ClienteJuego::ClienteJuego(Servidor& servidor_) 
    : ventana(1024, 600), servidor(servidor_), 
      controlador(ventana, servidor, juego)
{ }

int ClienteJuego::ejecutar() {
    while (!juego.esta_terminado()) {
        // Procesar eventos
        ventana.procesar_eventos(); // Cerrar ventana
        controlador.procesar_entrada(); // Servidor / Mouse / teclado
        
        // Actualizar el modelo del juego
        juego.actualizar(ventana.obtener_ms());
        
        // Renderizar el juego
        controlador.renderizar();

        // Mostrar los cambios
        ventana.actualizar();
    }
    
    return 0;
}

ClienteJuego::~ClienteJuego() { }

} // namespace cliente
