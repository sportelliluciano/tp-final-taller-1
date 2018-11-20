#include "cliente/modelo/hud/energia.h"

#include <sstream>
#include <string>

#include "cliente/video/ventana.h"

#define ANCHO_ENERGIA 150
#define ALTO_ENERGIA 30

#define PADDING_ENERGIA_X 10
#define PADDING_ENERGIA_Y 2

namespace cliente {

Energia::Energia(Juego& juego_) : juego(juego_) { }

int Energia::obtener_alto() const {
    return ALTO_ENERGIA;
}

int Energia::obtener_ancho() const {
    return ANCHO_ENERGIA;
}

void Energia::renderizar(Ventana& ventana, const Posicion& punto) {
    if (last_energia != juego.obtener_energia()) {
        std::stringstream s_energia;
        s_energia << "Energía: " << juego.obtener_energia();
        last_energia = juego.obtener_energia();
        s_last_energia = s_energia.str();
    }

    Textura& textura_energia = ventana
        .obtener_administrador_texturas()
        .crear_texto(s_last_energia);
    
    textura_energia.renderizar(punto.x + PADDING_ENERGIA_X, 
        punto.y + PADDING_ENERGIA_Y);
}

} // namespace cliente
