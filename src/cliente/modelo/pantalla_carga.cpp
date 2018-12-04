#include "cliente/modelo/pantalla_carga.h"

namespace cliente {

void PantallaCarga::renderizar(Ventana& ventana) {
    Rectangulo caja(0, 0, ventana.ancho(), ventana.alto());
    Textura texto = ventana
        .obtener_administrador_texturas()
        .crear_texto("Esperando a otros jugadores...", caja, COLOR_BLANCO, 
            TAM_FUENTE_GRANDE);
    
    texto.renderizar(ventana.ancho() / 2 - texto.obtener_ancho() / 2, 
        ventana.alto() / 2 - texto.obtener_alto() / 2);
}

}