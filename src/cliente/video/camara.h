#ifndef _CAMARA_H_
#define _CAMARA_H_

#include "cliente/video/posicion.h"
#include "cliente/video/rectangulo.h"

namespace cliente {

/**
 * \brief Cámara del juego.
 */
class Camara {
public:
    Camara();
    /**
     * \brief Crea una cámara de tamaño ancho x alto.
     */
    Camara(int ancho_, int alto_);

    /**
     * \brief Mueve la cámara a la posición absoluta (x, y).
     */
    void mover_camara(const Posicion& posicion_nueva);

    /**
     * \brief Desplaza la cámara a la posición (x + dx, y + dy), donde (x, y)
     *        es la posición actual de la cámara.
     */
    void desplazar_camara(const Posicion& delta);

    /**
     * \brief Traduce una posición lógica a una posición visual.
     */
    Posicion traducir_a_visual(const Posicion& posicion_logica);

    /**
     * \brief Traduce una posición visual a una posición lógica.
     */
    Posicion traducir_a_logica(const Posicion& posicion_visual);

    /**
     * \brief Devuelve true si al menos un fragmento del rectángulo es visible
     *        según la posición de la cámara.
     */
    bool es_visible(const Rectangulo& area);

    /**
     * \brief Devuelve un rectángulo que indica la porción de ventana vista.
     */
    Rectangulo obtener_vista();

private:
    /**
     * \brief Tamaño de la cámara.
     */
    int ancho, alto;

    /**
     * \brief Posición actual de la cámara.
     */
    int cam_x, cam_y;
};

} // namespace cliente

#endif // _CAMARA_H_ 
