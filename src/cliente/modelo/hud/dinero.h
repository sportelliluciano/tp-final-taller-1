#ifndef _DINERO_H_
#define _DINERO_H_

#include "cliente/modelo/sprite.h"
#include "cliente/modelo/juego.h"
#include "cliente/video/ventana.h"
#include "cliente/video/widgets/widget.h"

namespace cliente {

class Dinero : public Widget {
public:
    Dinero(Juego& juego_);

    /**
     * \brief Devuelve el ancho del widget, en píxeles.
     */  
    int obtener_alto() const override;

    /**
     * \brief Devuelve el ancho del widget, en píxeles.
     */
    int obtener_ancho() const override;

    /**
     * \brief Renderiza el widget en la posición (x, y).
     * 
     * Renderiza el widget en la posición (x, y) dada en píxeles, relativa a
     * la ventana (global).
     */
    void renderizar(Ventana& ventana, int x, int y) override;

private:
    Juego& juego;

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

    int ancho = 0, alto = 0;

    void setear_dinero(int nuevo_dinero);
};

} // namespace cliente

#endif // _DINERO_H_
