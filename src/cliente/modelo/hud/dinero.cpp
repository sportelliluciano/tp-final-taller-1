#include "cliente/modelo/hud/dinero.h"

#include <cmath>

#include "cliente/video/ventana.h"

#define DIGITO_0 27

namespace cliente {

Dinero::Dinero() {
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

void Dinero::renderizar(Ventana& ventana, int x, int y) {
    int digito;
    int dx = 0;
    for (digito=N_DIGITOS_DINERO-1; digito > 0; digito--) {
        if (digitos_dinero[digito] != 0)
            break;
        
        digitos[10].renderizar(ventana, x + dx, y);
        dx += digitos[10].obtener_textura(ventana).obtener_ancho();
    }

    for (; digito >= 0; digito--) {
        digitos[digitos_dinero[digito]].renderizar(ventana, x + dx, y);
        dx += digitos[digitos_dinero[digito]]
            .obtener_textura(ventana).obtener_ancho();
    }
}

} // namespace cliente
