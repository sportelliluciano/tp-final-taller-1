#ifndef _ENERGIA_H_
#define _ENERGIA_H_

#include "cliente/modelo/juego.h"
#include "cliente/video/widgets/widget.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Energia : public Widget {
public:
    Energia(Juego& juego_);

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
    void renderizar(Ventana& ventana, const Posicion& punto) override;

private:
    Juego& juego;
    int ancho = 0, alto = 0;
    int last_energia = -1;
    std::string s_last_energia;
};

} // namespace cliente

#endif // _ENERGIA_H_
