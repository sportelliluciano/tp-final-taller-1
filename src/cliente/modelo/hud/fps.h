#ifndef _FPS_H_
#define _FPS_H_

#include "cliente/video/widgets/widget.h"
#include "cliente/video/ventana.h"

namespace cliente {

class FPS : public Widget {
public:
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
    int ancho = 0, alto = 0;
    int last_fps = -1;
    std::string s_last_fps;
};

} // namespace cliente

#endif // _FPS_H_
