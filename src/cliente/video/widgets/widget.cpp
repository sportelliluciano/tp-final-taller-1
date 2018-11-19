#include "cliente/video/widgets/widget.h"

#include "cliente/video/ventana.h"

namespace cliente {

Widget* Widget::obtener_padre() {
    return padre;
}

void Widget::set_padre(Widget* nuevo_padre) {
    padre = nuevo_padre;
}

bool Widget::mouse_click_izquierdo(const Posicion&) {
    return true;
}

bool Widget::mouse_click_derecho(const Posicion&) {
    return true;
}

bool Widget::mouse_scroll(const Posicion& , int) {
    return true;
}

bool Widget::mouse_inicio_arrastre(const Posicion&) {
    return true;
}

bool Widget::mouse_movimiento(const Posicion&) {
    return true;
}

bool Widget::mouse_fin_arrastre(const Posicion&) {
    return true;
}

bool Widget::mouse_entra(const Posicion&) {
    return true;
}

bool Widget::mouse_sale(const Posicion&) {
    return true;
}

bool Widget::teclado_presionado(tecla_t) {
    return true;
}

bool Widget::teclado_suelto(tecla_t) {
    return true;
}

bool Widget::cerrar_ventana() {
    return true;
}

Widget::~Widget() {
    
}

} // cliente
