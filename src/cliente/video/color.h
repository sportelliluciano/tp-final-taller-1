#ifndef _COLOR_H_
#define _COLOR_H_

#include <SDL2/SDL.h>

namespace cliente {

class Color {
public:
    Color();

    /**
     * \brief Crea un nuevo color a partir de sus componentes RGBA.
     * 
     * Los valores para cada componente se limitarán al rango [0, 255].
     */
    Color(int r, int g, int b, int a = 0);

    /**
     * \brief Devuelve un puntero a la estructura SDL_Color.
     */
    const SDL_Color* obtener_color() const;

private:
    SDL_Color color;
};

} // namespace cliente

#endif // _COLOR_H_
