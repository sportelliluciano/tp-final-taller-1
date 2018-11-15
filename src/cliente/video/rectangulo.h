#ifndef _RECTANGULO_H_
#define _RECTANGULO_H_

#include <SDL2/SDL.h>

#include "cliente/video/posicion.h"

namespace cliente {

class Rectangulo {
public:
    Rectangulo() = default;

    /**
     * \brief Constructor. 
     * 
     * Crea un nuevo rectángulo con esquina en (x, y) y de tamaño w x h.
     */
    Rectangulo(int x, int y, int ancho, int alto);

    Rectangulo(const Posicion& esquina_sup_izq, 
        const Posicion& esquina_inf_der);

    Posicion esq_sup_izq, esq_inf_der;
    int w, h;


    SDL_Rect sdl_rect;
};

} // namespace cliente

#endif // _RECTANGULO_H_
