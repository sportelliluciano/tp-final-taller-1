#ifndef _TOSTADOR_H_
#define _TOSTADOR_H_

#include <list>
#include <string>

#include "cliente/modelo/hud/tostada.h"
#include "cliente/video/widgets/widget.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Genera mensajes de notificación / error sobre la ventana de juego.
 */
class Tostador : public Widget {
public:
    /**
     * \brief Crea un nuevo tostador que mostrá notificaciones en la posición
     *        (x, y).
     */
    Tostador(int x, int y);

    /**
     * \brief Devuelve el ancho del widget.
     * 
     * El tostador no tiene dimensiones y no recibe eventos.
     * Este método devuelve 0.
     */
    int obtener_ancho() const override;

    /**
     * \brief Devuelve el alto del widget.
     * 
     * El tostador no tiene dimensiones y no recibe eventos.
     * Este método devuelve 0.
     */
    int obtener_alto() const override;

    /**
     * \brief Renderiza los mensajes de notificación sobre la ventana.
     */
    void renderizar(Ventana& ventana, const Posicion& punto) override;

    /**
     * \brief Agrega una nueva notificación.
     */
    void hacer_tostada(const std::string& mensaje, int duracion_ms = 3000);

private:
    std::list<Tostada> tostadas;
    int pos_x, pos_y;
};

} // namespace cliente

#endif // _TOSTADOR_H_
