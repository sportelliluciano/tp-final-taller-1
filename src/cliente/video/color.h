#ifndef _COLOR_H_
#define _COLOR_H_

#include <SDL2/SDL.h>

#include <string>

#define COLOR_NEGRO Color(0, 0, 0, 255)
#define COLOR_BLANCO Color(255, 255, 255, 255)

namespace cliente {

class Color {
public:
    /**
     * \brief Crea un nuevo color, por defecto negro transparente.
     */
    Color();

    /**
     * \brief Crea un nuevo color a partir de un color SDL.
     */
    explicit Color(const SDL_Color* color_sdl);

    /**
     * \brief Crea un nuevo color a partir de sus componentes RGBA.
     * 
     * Los valores para cada componente se limitarán al rango [0, 255].
     */
    Color(int r, int g, int b, int a = 0);

    /**
     * \brief Obtiene una representación en cadena del color.
     * 
     * La representación es en la siguiente forma:
     * "rgba(123, 123, 123, 123)"
     */
    std::string str() const;

    /**
     * \brief Devuelve un puntero a la estructura SDL_Color.
     */
    const SDL_Color* obtener_color() const;

private:
    SDL_Color color;
};

} // namespace cliente

#endif // _COLOR_H_
