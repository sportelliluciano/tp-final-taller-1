#ifndef _HUD_H_
#define _HUD_H_

#include "cliente/modelo/hud/boton.h"
#include "cliente/modelo/hud/dinero.h"
#include "cliente/modelo/juego.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Interfaz de usuario del juego.
 */
class HUD {
public:
    HUD();
    
    void renderizar(Ventana& ventana, const Juego& juego);

private:
    Boton btn;
    Dinero dinero;
};

} // namespace cliente

#endif // _HUD_H_
