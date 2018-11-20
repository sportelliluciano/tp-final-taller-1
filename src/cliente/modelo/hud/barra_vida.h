#ifndef _BARRA_VIDA_H_
#define _BARRA_VIDA_H_

#include "cliente/video/ventana.h"

namespace cliente {

class BarraVida {
public:
    BarraVida() = default;

    void renderizar(Ventana& ventana, int x, int y, int vida);
};

} // namespace cliente

#endif // _BARRA_VIDA_H_
