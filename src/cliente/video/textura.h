#ifndef _TEXTURA_H_
#define _TEXTURA_H_

#include <SDL2/SDL.h>

#include "cliente/video/rectangulo.h"
#include "cliente/video/color.h"

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
    
    /**
     * \brief Recorta un fragmento de la superficie y lo renderiza.
     */
    void renderizar(int x, int y, const Rectangulo& seccion, 
        const Rectangulo& destino) const;
    
    void renderizar(int x, int y, const Rectangulo& seccion, 
        const Rectangulo& destino, Textura& renderizable) const;

    /**
     * \brief Elimina todo el contenido de la textura.
     */
    void limpiar();

    /**
     * \brief Elimina todo el contenido de la textura, dejandola del color 
     *        indicado.
     */
    void limpiar(const Color& color);

    /**
     * \brief Dibuja un rectángulo sin relleno sobre la textura.
     */
    void dibujar_rectangulo(const Rectangulo& rc, int ancho_linea, 
        const Color& color);

    /**
     * \brief Dibuja un rectángulo con relleno sobre la textura.
     */
    void rellenar_rectangulo(const Rectangulo& rc, const Color& color);
    
    Rectangulo obtener_rect() const;
    

private:
    Textura(SDL_Renderer *renderer, SDL_Texture* textura);

    SDL_Texture *textura;
    SDL_Renderer *renderer;
    SDL_Rect src;

    friend class Ventana;
    friend class AdministradorTexturas;
};

} // namespace cliente

#endif // _TEXTURA_H_
