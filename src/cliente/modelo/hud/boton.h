#ifndef _BOTON_H_
#define _BOTON_H_

#include "cliente/modelo/sprite.h"

namespace cliente {

/**
 * \brief Boton del HUD.
 */
class Boton {
public:
    Boton(int sprite_id, int x_, int y_);

    void renderizar(Ventana& ventana);

private:
    int sprite;
    int x, y;
};

} // namespace cliente

#endif // _BOTON_H_
