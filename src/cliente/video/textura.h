#ifndef _TEXTURA_H_
#define _TEXTURA_H_

#include <SDL2/SDL.h>

#include "cliente/video/rectangulo.h"

namespace cliente {

class Textura {
public:
    /**
     * \brief Constructor por defecto.
     */
    Textura();

    /**
     * \brief Constructor por movimiento.
     */
    Textura(Textura&& otro);
    Textura& operator=(Textura&& otro);

    /**
     * \brief Destructor.
     */
    ~Textura();
    
    /**
     * \brief Devuelve el alto de la textura.
     */
    int obtener_alto() const;

    /**
     * \brief Devuelve el ancho de la textura.
     */
    int obtener_ancho() const;

    /**
     * \brief Renderiza la textura en la posición (x, y).
     */
    void renderizar(int x, int y) const;

    /**
     * \brief Renderiza una sección de la textura en la posición (x, y).
     */
    void renderizar(int x, int y, const Rectangulo& seccion) const;

    /**
     * \brief Renderiza la textura en la posición (x, y), sobre la textura de
     *        destino.
     */
    void renderizar(int x, int y, Textura& destino) const;

    /**
     * \brief Renderiza una sección de la textura sobre la otra 
     *        textura en la posición (x, y).
     */
    void renderizar(int x, int y, const Rectangulo& seccion, 
        Textura& destino) const;
    

private:
    Textura(SDL_Renderer *renderer, SDL_Texture* textura);

    SDL_Texture *textura;
    SDL_Renderer *renderer;
    SDL_Rect src;

    friend class AdministradorTexturas;
};

} // namespace cliente

#endif // _TEXTURA_H_
