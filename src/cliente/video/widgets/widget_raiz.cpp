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

void WidgetRaiz::renderizar(Ventana& ventana, int x, int y) {
    if (hijo)
        hijo->renderizar(ventana, x, y);
}

bool WidgetRaiz::mouse_click_izquierdo(int x, int y) {
    if (hijo)
        return hijo->mouse_click_izquierdo(x, y);
    
    return false;
}

bool WidgetRaiz::mouse_click_derecho(int x, int y) {
    if (hijo)
        return hijo->mouse_click_derecho(x, y);
    
    return false;
}

bool WidgetRaiz::mouse_scroll(int x, int y, int dy) {
    if (hijo)
        return hijo->mouse_scroll(x, y, dy);
    
    return false;
}

bool WidgetRaiz::mouse_inicio_arrastre(int x, int y) {
    if (hijo)
        return hijo->mouse_inicio_arrastre(x, y);
    return false;
}

bool WidgetRaiz::mouse_movimiento(int x, int y) {
    if (hijo)
        return hijo->mouse_movimiento(x, y);
    return false;
}

bool WidgetRaiz::mouse_fin_arrastre(int x, int y) {
    if (hijo)
        return hijo->mouse_fin_arrastre(x, y);
    return false;
}

bool WidgetRaiz::mouse_entra(int x, int y) {
    if (hijo)
        return hijo->mouse_entra(x, y);
    return false;
}

bool WidgetRaiz::mouse_sale(int x, int y) {
    if (hijo)
        return hijo->mouse_sale(x, y);
    return false;
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
