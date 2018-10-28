#ifndef _RECTANGULO_H_
#define _RECTANGULO_H_

#include <SDL2/SDL.h>

namespace cliente {

class Rectangulo {
public:
    /**
     * \brief Constructor. 
     * 
     * Crea un nuevo rectángulo con esquina en (x, y) y de tamaño w x h.
     */
    Rectangulo(int x, int y, int w, int h);

    SDL_Rect sdl_rect;
};

} // namespace cliente

#endif // _RECTANGULO_H_
