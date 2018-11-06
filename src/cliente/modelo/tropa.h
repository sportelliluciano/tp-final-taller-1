#ifndef _TROPA_H_
#define _TROPA_H_

#include "cliente/modelo/sprite_animado.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Tropa {
public:
    Tropa();

    /**
     * \brief Renderiza la tropa.
     */
    void renderizar(Ventana& ventana);

private:
    static const int N_SPRITES = 32;
    SpriteAnimado sprites[N_SPRITES];

    float x, y;
    float x_destino, y_destino;
};

} // namespace cliente

#endif // _TROPA_H_
