#include "cliente/modelo/hud/boton_temporizado.h"

#include <sstream>
#include <string>

#include "cliente/modelo/sprite.h"
#include "cliente/video/ventana.h"

#define ANCHO_BOTON 60
#define ALTO_BOTON 50

namespace cliente {

BotonTemporizado::BotonTemporizado(int sprite) { 
    sprite_id = sprite;
}

int BotonTemporizado::obtener_alto() const {
    return ALTO_BOTON;
}

int BotonTemporizado::obtener_ancho() const {
    return ANCHO_BOTON;
}

void BotonTemporizado::set_cola(int nueva_cola) {
    cola = nueva_cola;
}

void BotonTemporizado::set_tiempo(int nuevo_tiempo) {
    segundos_restantes = nuevo_tiempo;
}

void BotonTemporizado::reiniciar() {
    cola = segundos_restantes = 0;
    con_filtro_gris = false;
    con_filtro_verde = false;
}

void BotonTemporizado::set_filtro(bool gris, bool verde) {
    con_filtro_gris = gris;
    con_filtro_verde = verde;
}


void BotonTemporizado::renderizar(Ventana& ventana, int x, int y) {
    Sprite(sprite_id).renderizar(ventana, x, y);
    if (con_filtro_gris) {
        ventana
            .obtener_administrador_texturas()
            .cargar_imagen("./assets/nuevos/filtro-boton-construccion.png")
            .renderizar(x, y);
    } else if (con_filtro_verde) {
        if (!ventana.obtener_administrador_texturas().contiene_textura("filtro-verde")) {
            ventana
                .obtener_administrador_texturas()
                .crear_textura("filtro-verde", ANCHO_BOTON, ALTO_BOTON)
                .limpiar(0, 255, 0, 128);
        }
        ventana
            .obtener_administrador_texturas()
            .obtener_textura("filtro-verde")
            .renderizar(x, y);
    }

    if (cola > 0) {
        std::stringstream s;
        s << cola;
        ventana
            .obtener_administrador_texturas()
            .crear_texto(s.str())
            .renderizar(x + 5, y);
    }

    if (segundos_restantes > 0) {
        std::stringstream s;
        s << segundos_restantes;
        ventana
            .obtener_administrador_texturas()
            .crear_texto(s.str())
            .renderizar(x + 5, y + ALTO_BOTON - 20);
    }
}

} // namespace cliente
