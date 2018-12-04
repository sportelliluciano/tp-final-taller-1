#ifndef _ENERGIA_H_
#define _ENERGIA_H_

#include "cliente/modelo/juego.h"
#include "cliente/video/widgets/widget.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Muestra la energía disponible del jugador.
 */
class Energia : public Widget {
public:
    Energia(Juego& juego_);

    /** Interfaz widget **/
    int obtener_alto() const override;
    int obtener_ancho() const override;
    void renderizar(Ventana& ventana, const Posicion& punto) override;

private:
    Juego& juego;
    int ancho = 0, alto = 0;
    int last_energia = -1;
    std::string s_last_energia;
};

} // namespace cliente

#endif // _ENERGIA_H_
