#include "cliente/video/widgets/caja_vertical.h"

#include <functional>
#include <list>
#include <vector>
#include <utility>

#include "cliente/video/widgets/contenedor.h"
#include "cliente/video/ventana.h"

namespace cliente {

CajaVertical::CajaVertical(int x_, int y_, int ancho_, int alto_) {
    ancho = ancho_;
    alto  = alto_;
    px = x_;
    py = y_;
}

void CajaVertical::empaquetar_arriba(Widget& hijo) {
    hijo.set_padre(this);
    arriba.push_back(&hijo);
}

void CajaVertical::empaquetar_abajo(Widget& hijo) {
    hijo.set_padre(this);
    abajo.push_back(&hijo);
}

void CajaVertical::reemplazar_widget(Widget& actual, Widget& nuevo) {
    for (auto it = arriba.begin(); it != arriba.end(); ++it) {
        Widget*& wx = *it;
        if (&actual == wx) {
            wx->set_padre(nullptr);
            wx = &nuevo;
            wx->set_padre(this);
            return;
        }
    }

    for (auto it = abajo.begin(); it != abajo.end(); ++it) {
        Widget*& wx = *it;
        if (&actual == wx) {
            wx->set_padre(nullptr);
            wx = &nuevo;
            wx->set_padre(this);
            return;
        }
    }
}

int CajaVertical::obtener_alto() const {
    return alto;
}

int CajaVertical::obtener_ancho() const {
    return ancho;
}

void CajaVertical::renderizar(Ventana& ventana, const Posicion& punto) {
    int pos_y = punto.y;
    int limite_y = obtener_alto() + punto.y;

    ventana.dibujar_rectangulo(punto.x, punto.y, punto.x + ancho, 
        punto.y + alto, 1);

    for (auto it=arriba.begin(); it != arriba.end(); ++it) {
        Widget& widget = **it;

        widget.renderizar(ventana, Posicion(punto.x, pos_y));
        pos_y += widget.obtener_alto();
    }

    pos_y = limite_y;

    for (auto it=abajo.rbegin(); it != abajo.rend(); ++it) {
        Widget& widget = **it;

        pos_y -= widget.obtener_alto();
        widget.renderizar(ventana, Posicion(punto.x, pos_y));
    }
}

std::vector<Widget*> CajaVertical::obtener_widgets() {
    std::vector<Widget*> resultado;
    resultado.insert(resultado.end(), arriba.begin(), arriba.end());    
    resultado.insert(resultado.end(), abajo.begin(), abajo.end());
    return resultado;
}

std::vector<std::pair<Posicion, Widget*>>
        CajaVertical::obtener_widgets(const Posicion& punto)
{
    std::vector<std::pair<Posicion, Widget*>> resultado;

    int pos_y = 0;
    int limite_y = obtener_alto();

    for (auto it=arriba.begin(); it != arriba.end(); ++it) {
        Widget* wx = *it;

        if ((pos_y <= punto.y) && (punto.y <= pos_y + wx->obtener_alto()))
            resultado.push_back({Posicion(punto.x, punto.y - pos_y), wx});

        pos_y += wx->obtener_alto();
    }

    pos_y = limite_y;

    for (auto it=abajo.rbegin(); it != abajo.rend(); ++it) {
        Widget* wx = *it;

        pos_y -= wx->obtener_alto();
        
        if ((pos_y <= punto.y) && (punto.y <= pos_y + wx->obtener_alto()))
            resultado.push_back({Posicion(punto.x, punto.y - pos_y), wx});
    }

    return resultado;
}

} // namespace cliente
