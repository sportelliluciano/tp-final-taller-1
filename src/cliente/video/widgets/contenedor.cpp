#include "cliente/video/widgets/contenedor.h"

#include <vector>
#include <utility>

namespace cliente {

std::vector<std::pair<Posicion, Widget*>>
    Contenedor::obtener_widgets(const Posicion& punto)
{
    std::vector<std::pair<Posicion, Widget*>> resultado;
    for (Widget *widget : obtener_widgets()) {
        resultado.push_back({punto, widget});
    }
    return resultado;
}

bool Contenedor::mouse_click_izquierdo(const Posicion& punto) {
    for (auto& par : obtener_widgets(punto)) {
        const Posicion& punto_relativo = par.first;
        Widget* widget = par.second;
        if (!widget->mouse_click_izquierdo(punto_relativo))
            return false;
    }
    return true;
}

bool Contenedor::mouse_click_derecho(const Posicion& punto) {
    for (auto& par : obtener_widgets(punto)) {
        const Posicion& punto_relativo = par.first;
        Widget* widget = par.second;
        if (!widget->mouse_click_derecho(punto_relativo))
            return false;
    }
    return true;
}

bool Contenedor::mouse_scroll(const Posicion& punto, int dy) {
    for (auto& par : obtener_widgets(punto)) {
        const Posicion& punto_relativo = par.first;
        Widget* widget = par.second;
        if (!widget->mouse_scroll(punto_relativo, dy))
            return false;
    }
    return true;
}

bool Contenedor::mouse_inicio_arrastre(const Posicion& punto) {
    for (auto& par : obtener_widgets(punto)) {
        const Posicion& punto_relativo = par.first;
        Widget* widget = par.second;
        if (!widget->mouse_inicio_arrastre(punto_relativo))
            return false;
    }
    return true;
}

bool Contenedor::mouse_movimiento(const Posicion& punto) {
    std::vector<std::pair<Posicion, Widget*>> widgets = obtener_widgets(punto);
    if ((widgets.size() == 0) && (ultimo_notificado_e_s)) {
        ultimo_notificado_e_s->mouse_sale(punto);
        ultimo_notificado_e_s = nullptr;
        return true;
    }

    for (auto& par : widgets) {
        const Posicion& punto_relativo = par.first;
        Widget* widget = par.second;

        if (!ultimo_notificado_e_s) {
            ultimo_notificado_e_s = widget;
            widget->mouse_entra(punto_relativo);
        } else if (ultimo_notificado_e_s != widget) {
            ultimo_notificado_e_s->mouse_sale(punto_relativo);
            ultimo_notificado_e_s = widget;
            widget->mouse_entra(punto_relativo);
        }

        if (!widget->mouse_movimiento(punto_relativo))
            return false;
    }
    return true;
}

bool Contenedor::mouse_fin_arrastre(const Posicion& punto) {
    for (auto& par : obtener_widgets(punto)) {
        const Posicion& punto_relativo = par.first;
        Widget* widget = par.second;
        if (!widget->mouse_fin_arrastre(punto_relativo))
            return false;
    }
    return true;
}

bool Contenedor::mouse_entra(const Posicion& punto) {
    for (auto& par : obtener_widgets(punto)) {
        const Posicion& punto_relativo = par.first;
        Widget* widget = par.second;
        if (!widget->mouse_entra(punto_relativo))
            return false;
    }
    return true;
}

bool Contenedor::mouse_sale(const Posicion& punto) {
    for (auto& par : obtener_widgets(punto)) {
        const Posicion& punto_relativo = par.first;
        Widget* widget = par.second;
        if (!widget->mouse_sale(punto_relativo))
            return false;
    }
    return true;
}

bool Contenedor::teclado_presionado(tecla_t tecla) {
    for (Widget *widget : obtener_widgets()) {
        if (!widget->teclado_presionado(tecla))
            return false;
    }
    return true;
}

bool Contenedor::teclado_suelto(tecla_t tecla) {
    for (Widget *widget : obtener_widgets()) {
        if (!widget->teclado_suelto(tecla))
            return false;
    }
    return true;
}

bool Contenedor::cerrar_ventana() {
    for (Widget *widget : obtener_widgets()) {
        if (!widget->cerrar_ventana())
            return false;
    }
    return true;
}

} // namespace cliente
