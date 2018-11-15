#include "cliente/modelo/pantalla_carga.h"

namespace cliente {

void PantallaCarga::renderizar(Ventana& ventana) {
    ventana
        .obtener_administrador_texturas()
        .crear_texto_nc("Esperando a otros jugadores...")
        .renderizar(ventana.ancho() / 2, ventana.alto() / 2);
}

}