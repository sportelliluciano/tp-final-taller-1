#include "cliente/video/widgets/caja_horizontal.h"

#include <iostream>
#include <functional>
#include <list>

#include "cliente/video/widgets/widget.h"
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

void CajaHorizontal::renderizar(Ventana& ventana, int x, int y) {
    int pos_x = x;
    int limite_x = obtener_ancho();

    ventana.dibujar_rectangulo(x, y, x + ancho, y + alto, 2);

    for (auto it=frente.begin(); it != frente.end(); ++it) {
        Widget& widget = **it;

        widget.renderizar(ventana, pos_x, y);
        pos_x += widget.obtener_ancho();
    }

    pos_x = limite_x;

    for (auto it=fondo.rbegin(); it != fondo.rend(); ++it) {
        Widget& widget = **it;

        pos_x -= widget.obtener_ancho();
        widget.renderizar(ventana, pos_x, y);
    }
}

void CajaHorizontal::buscar_por_x(int x, 
    std::function<void(Widget&, int)> callback) 
{
    int pos_x = 0;
    int limite_x = obtener_ancho();

    for (auto it=frente.begin(); it != frente.end(); ++it) {
        Widget& widget = **it;
        
        if ((pos_x <= x) && (x <= pos_x + widget.obtener_ancho()))
            callback(widget, pos_x);

        pos_x += widget.obtener_ancho();
    }

    pos_x = limite_x;

    for (auto it=fondo.rbegin(); it != fondo.rend(); ++it) {
        Widget& widget = **it;
        
        pos_x -= widget.obtener_ancho();

        if ((pos_x <= x) && (x <= pos_x + widget.obtener_ancho()))
            callback(widget, pos_x);
    }
}

bool CajaHorizontal::mouse_click_izquierdo(int x, int y) {
    std::cout << "CH: clic izq en (" << x << "," << y << ")" << std::endl;
    bool retval = true;
    buscar_por_x(x, 
        [&retval, this, x, y] (Widget& wx, int pos_x) {
            if (!wx.mouse_click_izquierdo(x - pos_x, y))
                retval = false;
        }
    );
    return retval;
}

bool CajaHorizontal::mouse_click_derecho(int x, int y) {
    bool retval = true;
    buscar_por_x(x, 
        [&retval, this, x, y] (Widget& wx, int pos_x) {
            if (!wx.mouse_click_derecho(x - pos_x, y))
                retval = false;
        }
    );
    return retval;
}

bool CajaHorizontal::mouse_scroll(int x, int y, int dy) {
    bool retval = true;
    buscar_por_x(x, 
        [&retval, this, x, y, dy] (Widget& wx, int pos_x) {
            if (!wx.mouse_scroll(x - pos_x, y, dy))
                retval = false;
        }
    );
    return retval;
}

bool CajaHorizontal::mouse_inicio_arrastre(int x, int y) {
    bool retval = true;
    buscar_por_x(x, 
        [&retval, this, x, y] (Widget& wx, int pos_x) {
            if (!wx.mouse_inicio_arrastre(x - pos_x, y))
                retval = false;
        }
    );
    return retval;
}

bool CajaHorizontal::mouse_movimiento(int x, int y) {
    bool retval = true;
    buscar_por_x(x, 
        [&retval, this, x, y] (Widget& wx, int pos_x) {
            if (!ultimo_notificado_movimiento) {
                ultimo_notificado_movimiento = &wx;
                wx.mouse_entra(x - pos_x, y);
            } else if (ultimo_notificado_movimiento != &wx) {
                ultimo_notificado_movimiento->mouse_sale(x - pos_x, y);
                ultimo_notificado_movimiento = &wx;
                wx.mouse_entra(x - pos_x, y);
            }

            if (!wx.mouse_movimiento(x - pos_x, y))
                retval = false;
        }
    );
    return retval;
}

bool CajaHorizontal::mouse_fin_arrastre(int x, int y) {
    bool retval = true;
    buscar_por_x(x, 
        [&retval, this, x, y] (Widget& wx, int pos_x) {
            if (!wx.mouse_fin_arrastre(x - pos_x, y))
                retval = false;
        }
    );
    return retval;
}

bool CajaHorizontal::mouse_entra(int x, int y) {
    bool retval = true;
    buscar_por_x(x,
        [&retval, this, x, y] (Widget& wx, int pos_x) {
            if (!wx.mouse_entra(x - pos_x, y))
                retval = false;
        }
    );
    return retval;
}

bool CajaHorizontal::mouse_sale(int x, int y) {
    bool retval = true;
    buscar_por_x(x,
        [&retval, this, x, y] (Widget& wx, int pos_x) {
            if (!wx.mouse_sale(x - pos_x, y))
                retval = false;
        }
    );
    return retval;
}

} // namespace cliente
