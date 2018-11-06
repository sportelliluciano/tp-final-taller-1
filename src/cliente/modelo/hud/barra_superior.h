#ifndef _BARRA_SUPERIOR_H_
#define _BARRA_SUPERIOR_H_

#include "cliente/modelo/juego.h"
#include "cliente/modelo/hud/dinero.h"
#include "cliente/modelo/hud/botonera.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Barra superior del HUD.
 */
class BarraSuperior {
public:
    BarraSuperior(int ancho_ventana);

    void click_derecho(int x, int y);
    void click_izquierdo(int x, int y);
    void renderizar(Ventana& ventana, const Juego& juego);
    bool contiene_punto(int x, int y) const;

private:
    int ancho;
    Dinero dinero;
    Botonera botonera;
};

} // namespace cliente

#endif // _BARRA_SUPERIOR_H_
