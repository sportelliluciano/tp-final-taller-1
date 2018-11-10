#include "cliente/video/widgets/caja_vertical.h"

#include <iostream>
#include <list>

#include "cliente/video/widgets/widget.h"
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

int CajaVertical::obtener_alto() const {
    return alto;
}

int CajaVertical::obtener_ancho() const {
    return ancho;
}

void CajaVertical::renderizar(Ventana& ventana, int x, int y) {
    int pos_y = y;
    int limite_y = obtener_alto() + y;

    ventana.dibujar_rectangulo(x, y, x + ancho, y + alto, 1);

    for (auto it=arriba.begin(); it != arriba.end(); ++it) {
        Widget& widget = **it;

        widget.renderizar(ventana, x, pos_y);
        pos_y += widget.obtener_alto();
    }

    pos_y = limite_y;

    for (auto it=abajo.rbegin(); it != abajo.rend(); ++it) {
        Widget& widget = **it;

        pos_y -= widget.obtener_alto();
        widget.renderizar(ventana, x, pos_y);
    }
}

void CajaVertical::buscar_por_y(int y, std::function<void(Widget&, int)> cb) {
    int pos_y = 0;
    int limite_y = obtener_alto();

    for (auto it=arriba.begin(); it != arriba.end(); ++it) {
        Widget& widget = **it;

        if ((pos_y <= y) && (y <= pos_y + widget.obtener_alto()))
            cb(widget, pos_y);

        pos_y += widget.obtener_alto();
    }

    pos_y = limite_y;

    for (auto it=abajo.rbegin(); it != abajo.rend(); ++it) {
        Widget& widget = **it;

        pos_y -= widget.obtener_alto();
        
        if ((pos_y <= y) && (y <= pos_y + widget.obtener_alto()))
            cb(widget, pos_y);
    }
}

bool CajaVertical::mouse_click_izquierdo(int x, int y) {
    std::cout << "CV: clic izq en (" << x << "," << y << ")" << std::endl;
    bool retval = true;
    buscar_por_y(y, 
        [&retval, this, x, y] (Widget& wx, int pos_y) {
            if (!wx.mouse_click_izquierdo(x, y - pos_y))
                retval = false;
        }
    );
    return retval;
}

bool CajaVertical::mouse_click_derecho(int x, int y) {
    bool retval = true;
    buscar_por_y(y, 
        [&retval, this, x, y] (Widget& wx, int pos_y) {
            if (!wx.mouse_click_derecho(x, y - pos_y))
                retval = false;
        }
    );
    return retval;
}

bool CajaVertical::mouse_scroll(int x, int y, int dy) {
    bool retval = true;
    buscar_por_y(y, 
        [&retval, this, x, y, dy] (Widget& wx, int pos_y) {
            if (!wx.mouse_scroll(x, y - pos_y, dy))
                retval = false;
        }
    );
    return retval;
}

bool CajaVertical::mouse_inicio_arrastre(int x, int y) {
    bool retval = true;
    buscar_por_y(y,
        [&retval, this, x, y] (Widget& wx, int pos_y) {
            if (!wx.mouse_inicio_arrastre(x, y - pos_y))
                retval = false;
        }
    );
    return retval;
}

bool CajaVertical::mouse_movimiento(int x, int y) {
    bool retval = true;
    buscar_por_y(y,
        [&retval, this, x, y] (Widget& wx, int pos_y) {
            if (!ultimo_notificado_movimiento) {
                ultimo_notificado_movimiento = &wx;
                wx.mouse_entra(x, y - pos_y);
            } else if (ultimo_notificado_movimiento != &wx) {
                ultimo_notificado_movimiento->mouse_sale(x, y - pos_y);
                ultimo_notificado_movimiento = &wx;
                wx.mouse_entra(x, y - pos_y);
            }

            if (!wx.mouse_movimiento(x, y - pos_y))
                retval = false;
        }
    );
    return retval;
}

bool CajaVertical::mouse_fin_arrastre(int x, int y) {
    bool retval = true;
    buscar_por_y(y,
        [&retval, this, x, y] (Widget& wx, int pos_y) {
            if (!wx.mouse_fin_arrastre(x, y - pos_y))
                retval = false;
        }
    );
    return retval;
}

bool CajaVertical::mouse_entra(int x, int y) {
    bool retval = true;
    buscar_por_y(y,
        [&retval, this, x, y] (Widget& wx, int pos_y) {
            if (!wx.mouse_entra(x, y - pos_y))
                retval = false;
        }
    );
    return retval;
}

bool CajaVertical::mouse_sale(int x, int y) {
    bool retval = true;
    buscar_por_y(y,
        [&retval, this, x, y] (Widget& wx, int pos_y) {
            if (!wx.mouse_sale(x, y - pos_y))
                retval = false;
        }
    );
    return retval;
}

} // namespace cliente
