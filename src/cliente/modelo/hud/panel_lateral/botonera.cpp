#include "cliente/modelo/hud/panel_lateral/botonera.h"

#define PADDING_PX 10
#define SPACING_PX 4
namespace cliente {

Botonera::Botonera(int ancho_, int alto_) {
    ancho = ancho_;
    alto = alto_;
    padding_x = PADDING_PX;
    padding_y = PADDING_PX;
    spacing = SPACING_PX;
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

std::vector<Widget*> Botonera::obtener_widgets() {
    std::vector<Widget*> resultado;
    resultado.insert(resultado.end(), widgets.begin(), widgets.end());
    return resultado;
}

std::vector<std::pair<Posicion, Widget*>>
    Botonera::obtener_widgets(const Posicion& punto)
{
    std::vector<std::pair<Posicion, Widget*>> resultado;
    calcular_padding();
    int dx = padding_x, dy = padding_y, max_alto = 0;
    for (Widget* wx : widgets) {
        if (dx + wx->obtener_ancho() > ancho) {
            dx = padding_x;
            dy += max_alto + spacing;
        }

        Rectangulo area(dx, dy, wx->obtener_ancho(), wx->obtener_alto());

        if (area.contiene_punto(punto)) {
            resultado.push_back({Posicion(punto.x - dx, punto.y - dy), wx});
        }

        if (wx->obtener_alto() > max_alto)
            max_alto = wx->obtener_alto();
        
        dx += wx->obtener_ancho() + spacing;
    }
    return resultado;
}

void Botonera::renderizar(Ventana& ventana, const Posicion& punto) {
    calcular_padding();

    int dx = padding_x, dy = padding_y, max_alto = 0;
    for (Widget* wx : widgets) {
        if (dx + wx->obtener_ancho() > ancho) {
            dx = padding_x;
            dy += max_alto + spacing;
        }

        wx->renderizar(ventana, Posicion(punto.x + dx, punto.y + dy));
        if (wx->obtener_alto() > max_alto)
            max_alto = wx->obtener_alto();
        

        dx += wx->obtener_ancho() + spacing;
    }
}

} // namespace cliente