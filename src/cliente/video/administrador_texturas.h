#ifndef _ADMINISTRADOR_TEXTURAS_H_
#define _ADMINISTRADOR_TEXTURAS_H_

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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
     * \brief Crea una nueva textura con el texto indicado-
     */
    Textura crear_texto(const std::string& texto);

    /**
     * \brief Destructor.
     * 
     * Libera todas las texturas almacenadas en caché.
     */
    ~AdministradorTexturas();

private:
    SDL_Renderer *renderer;
    TTF_Font* fuente;
    std::unordered_map<std::string, Textura> texturas;
};

} // namespace cliente

#endif // _ADMINISTRADOR_TEXTURAS_H_
