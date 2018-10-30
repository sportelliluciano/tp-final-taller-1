#ifndef _HUD_H_
#define _HUD_H_

#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Interfaz de usuario del juego.
 */
class HUD {
public:
    HUD();
    
    void renderizar(Ventana& ventana);
};

} // namespace cliente

#endif // _HUD_H_
