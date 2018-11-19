#include "cliente/video/widgets/widget_raiz.h"

#include "cliente/video/widgets/widget.h"
#include "cliente/video/ventana.h"

namespace cliente {

WidgetRaiz::WidgetRaiz(int ancho_ventana, int alto_ventana) {
    ancho = ancho_ventana;
    alto = alto_ventana;
}

int WidgetRaiz::obtener_alto() const {
    return alto;
}

int WidgetRaiz::obtener_ancho() const {
    return ancho;
}

void WidgetRaiz::renderizar(Ventana& ventana, const Posicion& punto) {
    if (hijo)
        hijo->renderizar(ventana, punto);
}

std::vector<Widget*> WidgetRaiz::obtener_widgets() {
    std::vector<Widget*> resultado;
    if (hijo)
        resultado.push_back(hijo);
    
    return resultado;
}

void WidgetRaiz::setear_hijo(Widget* widget) {
    hijo = widget;
    hijo->set_padre(this);
}

Widget* WidgetRaiz::obtener_padre() {
    throw std::runtime_error("No se puede obtener el padre de la raíz");
}

void WidgetRaiz::set_padre(Widget*) {
    throw std::runtime_error("No se puede setear el padre de la raíz");
}

WidgetRaiz::~WidgetRaiz() { }

} // namespace cliente
