#include "cliente/modelo/hud/botonera.h"

#include "cliente/modelo/hud/boton_construccion.h"

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

void Botonera::agregar_widget(Widget& widget) {
    widget.set_padre(this);
    widgets.push_back(&widget);
}

void Botonera::calcular_padding() {
    int dx = padding_y;
    for (Widget* wx : widgets) {
        if (dx + wx->obtener_ancho() >= ancho) {
            padding_x = (padding_y / 2) + ((ancho - dx) / 2);
            return;
        }

        dx += wx->obtener_ancho() + spacing;
    }
}

bool Botonera::mouse_click_izquierdo(int x, int y) {
    calcular_padding();
    int dx = padding_x, dy = padding_y, max_alto = 0;
    for (Widget* wx : widgets) {
        if (dx + wx->obtener_ancho() > ancho) {
            dx = padding_x;
            dy += max_alto + spacing;
        }

        if ((dy <= y) && (y <= dy + wx->obtener_alto())) {
            if ((dx <= x) && (x <= dx + wx->obtener_ancho())) {
                wx->mouse_click_izquierdo(x, y);
                return false;
            }
        }

        if (wx->obtener_alto() > max_alto)
            max_alto = wx->obtener_alto();

        dx += wx->obtener_ancho() + spacing;
    }

    return true;
}

void Botonera::renderizar(Ventana& ventana, int x, int y) {
    calcular_padding();
    ventana.dibujar_rectangulo(x, y, x + ancho, y + alto);

    int dx = padding_x, dy = padding_y, max_alto = 0;
    for (Widget* wx : widgets) {
        if (dx + wx->obtener_ancho() > ancho) {
            dx = padding_x;
            dy += max_alto + spacing;
        }

        wx->renderizar(ventana, x + dx, y + dy);
        ventana.dibujar_rectangulo(
            x + dx, y + dy, 
            x + dx + wx->obtener_ancho(), y + dy + wx->obtener_alto());
        
        if (wx->obtener_alto() > max_alto)
            max_alto = wx->obtener_alto();
        

        dx += wx->obtener_ancho() + spacing;
    }
}

} // namespace cliente