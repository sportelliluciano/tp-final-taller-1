#ifndef _ADMINISTRADOR_TEXTURAS_H_
#define _ADMINISTRADOR_TEXTURAS_H_

#include <unordered_map>

#include <SDL2/SDL.h>

#include "cliente/video/textura.h"

namespace cliente {

class AdministradorTexturas {
public:
    /**
     * \brief Crea un nuevo administrador de texturas para un determinado 
     *        renderer.
     */
    AdministradorTexturas(SDL_Renderer *renderer_);

    /**
     * \brief Carga una imagen desde un archivo a una textura.
     * 
     * Almacena las texturas en una caché de modo que al volver a usarlas 
     * no haya que levantarlas desde el disco.
     */
    const Textura& cargar_imagen(const char *img);

    /**
     * \brief Crea una nueva textura vacía.
     */
    Textura crear_textura(int w, int h);

    /**
     * \brief Destructor.
     * 
     * Libera todas las texturas almacenadas en caché.
     */
    ~AdministradorTexturas();

private:
    SDL_Renderer *renderer;
    std::unordered_map<const char*, Textura> texturas;
};

} // namespace cliente

#endif // _ADMINISTRADOR_TEXTURAS_H_
