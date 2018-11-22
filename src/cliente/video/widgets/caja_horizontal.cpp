#include "cliente/video/widgets/caja_horizontal.h"

#include <functional>
#include <list>
#include <vector>
#include <utility>

#include "cliente/video/widgets/contenedor.h"
#include "cliente/video/ventana.h"

namespace cliente {

CajaHorizontal::CajaHorizontal(int x_, int y_, int ancho_, int alto_) {
    ancho = ancho_;
    alto = alto_;
    px = x_;
    py = y_;
}

void CajaHorizontal::empaquetar_al_frente(Widget& hijo) {
    hijo.set_padre(this);
    frente.push_back(&hijo);
}

void CajaHorizontal::empaquetar_al_fondo(Widget& hijo) {
    hijo.set_padre(this);
    fondo.push_back(&hijo);
}

int CajaHorizontal::obtener_alto() const {
    return alto;
}

int CajaHorizontal::obtener_ancho() const {
    return padre->obtener_ancho();
}

void CajaHorizontal::renderizar(Ventana& ventana, const Posicion& punto) {
    int pos_x = punto.x;
    int limite_x = obtener_ancho();

#ifdef DEPURACION_DIBUJO
    ventana.dibujar_rectangulo(punto.x, punto.y, punto.x + ancho, 
        punto.y + alto, 2);
#endif

    for (auto it=frente.begin(); it != frente.end(); ++it) {
        Widget& widget = **it;

        widget.renderizar(ventana, Posicion(pos_x, punto.y));
        pos_x += widget.obtener_ancho();
    }

    pos_x = limite_x;

    for (auto it=fondo.rbegin(); it != fondo.rend(); ++it) {
        Widget& widget = **it;

        pos_x -= widget.obtener_ancho();
        widget.renderizar(ventana, Posicion(pos_x, punto.y));
    }
}

std::vector<Widget*> CajaHorizontal::obtener_widgets() {
    std::vector<Widget*> resultado;
    resultado.insert(resultado.end(), frente.begin(), frente.end());
    resultado.insert(resultado.end(), fondo.begin(), fondo.end());
    return resultado;
}

std::vector<std::pair<Posicion, Widget*>>
        CajaHorizontal::obtener_widgets(const Posicion& punto) 
{
    std::vector<std::pair<Posicion, Widget*>> resultado;

    int pos_x = 0;
    int limite_x = obtener_ancho();

    for (auto it=frente.begin(); it != frente.end(); ++it) {
        Widget* wx = *it;
        
        if ((pos_x <= punto.x) && (punto.x <= pos_x + wx->obtener_ancho()))
            resultado.push_back({Posicion(punto.x - pos_x, punto.y), wx});

        pos_x += wx->obtener_ancho();
    }

    pos_x = limite_x;

    for (auto it=fondo.rbegin(); it != fondo.rend(); ++it) {
        Widget* wx = *it;
        
        pos_x -= wx->obtener_ancho();

        if ((pos_x <= punto.x) && (punto.x <= pos_x + wx->obtener_ancho()))
            resultado.push_back({Posicion(punto.x - pos_x, punto.y), wx});
    }

    return resultado;
}

} // namespace cliente
