#ifndef _BARRA_VIDA_H_
#define _BARRA_VIDA_H_

#include "cliente/video/ventana.h"

namespace cliente {

class BarraVida {
public:
    BarraVida();

    void set_ancho(int nuevo_ancho);
    void renderizar(Ventana& ventana, int x, int y, int vida, int maximo);

private:
    int ancho;
};

} // namespace cliente

#endif // _BARRA_VIDA_H_
