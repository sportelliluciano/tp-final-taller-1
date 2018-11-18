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

    /**
     * \brief Crea un nuevo rectángulo a partir de dos esquinas opuestas.
     * 
     * Es indiferente si las esquinas son la superior derecha e inferior 
     * izquierda o la superior izquierda e inferior derecha.
     */
    Rectangulo(const Posicion& esquina_a, const Posicion& esquina_b);

    /**
     * \brief Devuelve true si el punto está contenido en el rectángulo.
     */
    bool contiene_punto(const Posicion& punto) const;

    /**
     * \brief Getter/setter para el x de la esquina superior del rectángulo
     */
    int x() const;
    void x(int nuevo_x);
    
    /**
     * \brief Getter/setter para el y de la esquina superior del rectángulo
     */
    int y() const;
    void y(int nuevo_y);

    /**
     * \brief Getter/setter para el ancho del rectángulo
     */
    int ancho() const;
    void ancho(int nuevo_ancho);
    
    /**
     * \brief Getter/setter para el alto del rectángulo
     */
    int alto() const;
    void alto(int nuevo_alto);

    /**
     * \brief Getter/setter para la esquina superior izquierda del rectángulo
     */
    Posicion esquina_sup_izq() const;
    void esquina_sup_izq(const Posicion& nueva_esquina_sup_izq);

    /**
     * \brief Getter/setter para la esquina inferior derecha del rectángulo
     */
    Posicion esquina_inf_der() const;
    void esquina_inf_der(const Posicion& nueva_esquina_inf_der);

    /**
     * \brief Getter/setter para el rectángulo SDL representando al rectángulo.
     */
    const SDL_Rect& rect() const; 
    void rect(const SDL_Rect& nuevo_rect);

private:
    SDL_Rect sdl_rect;
};

} // namespace cliente

#endif // _RECTANGULO_H_
