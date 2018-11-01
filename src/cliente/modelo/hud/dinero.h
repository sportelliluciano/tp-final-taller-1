#ifndef _DINERO_H_
#define _DINERO_H_

#include "cliente/modelo/sprite.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Dinero {
public:
    Dinero();
    void setear_dinero(int nuevo_dinero);
    void renderizar(Ventana& ventana, int x, int y);

private:
    // Tomado del enunciado
    static const int N_DIGITOS_DINERO = 8;

    /**
     * \brief Valor de cada dígito del dinero actual.
     */
    int digitos_dinero[N_DIGITOS_DINERO];

    /**
     * \brief Dígitos a mostrar.
     */
    Sprite digitos[11];
};

} // namespace cliente

#endif // _DINERO_H_
