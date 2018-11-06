#ifndef _PANEL_LATERAL_H_
#define _PANEL_LATERAL_H_

#include "cliente/modelo/hud/botonera.h"
#include "cliente/modelo/juego.h"
#include "cliente/sonido/sonido.h"
#include "cliente/video/ventana.h"

namespace cliente {

class PanelLateral {
public:
    PanelLateral(Juego& juego_);

    void click_derecho(int x, int y);
    void click_izquierdo(int x, int y);
    void renderizar(Ventana& ventana, const Juego&);
    bool contiene_punto(int x, int y) const;

private:
    Botonera botonera;
    Juego& juego;
    int base_x = 0, base_botonera_y = 0;
    Sonido sonido;
};

} // namespace cliente

#endif // _PANEL_LATERAL_H_
