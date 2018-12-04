#ifndef _DINERO_H_
#define _DINERO_H_

#include "cliente/modelo/sprite.h"
#include "cliente/modelo/juego.h"
#include "cliente/video/ventana.h"
#include "cliente/video/widgets/widget.h"

namespace cliente {

/**
 * \brief Muestra el dinero disponible del jugador.
 */
class Dinero : public Widget {
public:
    Dinero(Juego& juego_);

    /** Interfaz widget **/
    int obtener_alto() const override;
    int obtener_ancho() const override;
    void renderizar(Ventana& ventana, const Posicion& punto) override;

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
