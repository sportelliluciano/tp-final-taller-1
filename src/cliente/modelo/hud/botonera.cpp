#include "cliente/modelo/hud/botonera.h"

#include <iostream>

#include "cliente/modelo/hud/boton.h"

namespace cliente {

Botonera::Botonera(int ancho_, int alto_) {
    ancho = ancho_;
    alto = alto_;
    padding_x = 10;
    padding_y = 0;
    spacing = 4;
}

int Botonera::obtener_alto() const {
    return alto;
}

int Botonera::obtener_ancho() const {
    return ancho;
}

Boton* Botonera::crear_boton() {
    botones.emplace_back(Boton());
    return &botones.back();
}

void Botonera::calcular_padding() {
    int dx = padding_y;
    for (auto it = botones.begin(); it != botones.end(); ++it) {
        Boton& btn = *it;

        if (dx + btn.obtener_ancho() >= ancho) {
            padding_x = (padding_y / 2) + ((ancho - dx) / 2);
            return;
        }

        dx += btn.obtener_ancho() + spacing;
    }
}

bool Botonera::mouse_click_izquierdo(int x, int y) {
    std::cout << "botonera click" << std::endl;
    calcular_padding();
    int dx = padding_x, dy = padding_y, max_alto = 0;
    for (auto it = botones.begin(); it != botones.end(); ++it) {
        Boton& btn = *it;
        
        if (dx + btn.obtener_ancho() > ancho) {
            dx = padding_x;
            dy += max_alto + spacing;
        }

        if ((dy <= y) && (y <= dy + btn.obtener_alto())) {
            if ((dx <= x) && (x <= dx + btn.obtener_ancho())) {
                btn.mouse_click_izquierdo(x, y);
                return false;
            }
        }

        if (btn.obtener_alto() > max_alto)
            max_alto = btn.obtener_alto();

        dx += btn.obtener_ancho() + spacing;
    }

    return true;
}

void Botonera::renderizar(Ventana& ventana, int x, int y) {
    calcular_padding();
    ventana.dibujar_rectangulo(x, y, x + ancho, y + alto);

    int dx = padding_x, dy = padding_y, max_alto = 0;
    for (auto it = botones.begin(); it != botones.end(); ++it) {
        Boton& btn = *it;

        if (dx + btn.obtener_ancho() > ancho) {
            dx = padding_x;
            dy += max_alto + spacing;
        }

        btn.renderizar(ventana, x + dx, y + dy);
        ventana.dibujar_rectangulo(
            x + dx, y + dy, 
            x + dx + btn.obtener_ancho(), y + dy + btn.obtener_alto());
        
        if (btn.obtener_alto() > max_alto)
            max_alto = btn.obtener_alto();
        

        dx += btn.obtener_ancho() + spacing;
    }
}

} // namespace cliente