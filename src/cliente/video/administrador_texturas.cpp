#include "cliente/video/administrador_texturas.h"

#include <SDL2/SDL.h>

#include "cliente/video/error_sdl.h"
#include "cliente/video/log.h"
#include "cliente/video/textura.h"

namespace cliente {

AdministradorTexturas::AdministradorTexturas(SDL_Renderer *renderer_) {
    renderer = renderer_;
}

const Textura& AdministradorTexturas::cargar_imagen(const char *img) {
    if (texturas.count(img) != 0)
        return texturas.find(img)->second;
    
    SDL_Surface *superficie = SDL_LoadBMP(img);
    if (!superficie)
        throw ErrorSDL("SDL_LoadBMP");
    
    SDL_SetColorKey(superficie, SDL_TRUE, 0);
    
    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderer, superficie);
    SDL_FreeSurface(superficie);

    if (!textura)
        throw ErrorSDL("SDL_CreateTextureFromSurface");

    texturas.emplace(img, Textura(renderer, textura));
    
    log_depuracion("Cargado %s", img);
    return texturas.find(img)->second;
}

Textura AdministradorTexturas::crear_textura(int w, int h) { 
    SDL_Texture *textura = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, w, h);
    
    if (!textura)
        throw ErrorSDL("SDL_CreateTexture");
    
    return Textura(renderer, textura);
}

AdministradorTexturas::~AdministradorTexturas() { }

} // namespace cliente
