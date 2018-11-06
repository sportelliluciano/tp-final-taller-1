#include "cliente/modelo/hud.h"

#include <iostream>

#include "cliente/modelo/juego.h"
#include "cliente/modelo/hud/barra_superior.h"
#include "cliente/modelo/hud/panel_lateral.h"
#include "cliente/video/ventana.h"

namespace cliente {

HUD::HUD(Ventana& ventana, Juego& juego) 
: barra_superior(ventana.ancho()), panel_lateral(juego)
{}

void HUD::click_derecho(int x, int y) {
    if (barra_superior.contiene_punto(x, y))
        barra_superior.click_derecho(x, y);
    else if (panel_lateral.contiene_punto(x, y))
        panel_lateral.click_derecho(x, y);
}

void HUD::click_izquierdo(int x, int y) {
    std::cout << "Click en HUD" << std::endl;
    if (barra_superior.contiene_punto(x, y))
        barra_superior.click_izquierdo(x, y);
    else if (panel_lateral.contiene_punto(x, y))
        panel_lateral.click_izquierdo(x, y);
}

bool HUD::contiene_punto(int x, int y) {
    return panel_lateral.contiene_punto(x, y) ||
        barra_superior.contiene_punto(x, y);
}

void HUD::renderizar(Ventana& ventana, const Juego& juego) {
    panel_lateral.renderizar(ventana, juego);
    barra_superior.renderizar(ventana, juego);
}

} // namespace cliente
