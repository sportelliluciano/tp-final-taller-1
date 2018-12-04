#ifndef _FPS_H_
#define _FPS_H_

#include "cliente/video/widgets/widget.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Widget que muestra los cuadros por segundo a los cuales se está
 *        renderizando la ventana.
 */
class FPS : public Widget {
public:
    FPS() = default;
    
    /** Interfaz widget **/
    int obtener_alto() const override;
    int obtener_ancho() const override;
    void renderizar(Ventana& ventana, const Posicion& punto) override;

private:
    int ancho = 0, alto = 0;
    int last_fps = -1;
    std::string s_last_fps;
};

} // namespace cliente

#endif // _FPS_H_
