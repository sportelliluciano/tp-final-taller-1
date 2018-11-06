#ifndef _HUD_H_
#define _HUD_H_

#include "cliente/modelo/hud/barra_superior.h"
#include "cliente/modelo/hud/panel_lateral.h"
#include "cliente/modelo/juego.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Interfaz de usuario del juego.
 */
class HUD {
public:
    HUD(Ventana& ventana, Juego& juego);
    
    void click_derecho(int x, int y);
    
    void click_izquierdo(int x, int y);

    bool contiene_punto(int x, int y);

    void renderizar(Ventana& ventana, const Juego& juego);

private:
    BarraSuperior barra_superior;
    PanelLateral panel_lateral;
};

} // namespace cliente

#endif // _HUD_H_
