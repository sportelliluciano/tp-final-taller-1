#include "cliente/modelo/hud/dinero.h"

#include <cmath>

#include "cliente/video/ventana.h"

#define DIGITO_0 27

#define ANCHO_DINERO 100
#define ALTO_DINERO 30

#define PADDING_DINERO_X 10
#define PADDING_DINERO_Y 7

namespace cliente {

Dinero::Dinero(Juego& juego_) : juego(juego_) {
    for (int i=0;i<11;i++)
        digitos[i] = Sprite(DIGITO_0 + i, 0, 0);
    
    for (int i=0;i<N_DIGITOS_DINERO;i++)
        digitos_dinero[i] = 0;
}

void Dinero::setear_dinero(int nuevo_dinero) {
    if (nuevo_dinero > pow(10, N_DIGITOS_DINERO))
        throw std::runtime_error("El dinero no es representable");
    
    for (int i=0;i<N_DIGITOS_DINERO;i++) {
        digitos_dinero[i] = nuevo_dinero % 10;
        nuevo_dinero /= 10;
    }
}

int Dinero::obtener_alto() const {
    return ALTO_DINERO;
}

int Dinero::obtener_ancho() const {
    return ANCHO_DINERO;
}

void Dinero::renderizar(Ventana& ventana, int x, int y) {
    setear_dinero(juego.obtener_dinero());
    int digito;
    int dx = 0;
    for (digito=N_DIGITOS_DINERO-1; digito > 0; digito--) {
        if (digitos_dinero[digito] != 0)
            break;
        
        digitos[10].renderizar(ventana, 
            x + PADDING_DINERO_X + dx, y + PADDING_DINERO_Y);
        dx += digitos[10].obtener_textura(ventana).obtener_ancho();
    }

    for (; digito >= 0; digito--) {
        digitos[digitos_dinero[digito]].renderizar(ventana, 
            x + PADDING_DINERO_X + dx, y + PADDING_DINERO_Y);
        dx += digitos[digitos_dinero[digito]]
            .obtener_textura(ventana).obtener_ancho();
    }
}

} // namespace cliente
