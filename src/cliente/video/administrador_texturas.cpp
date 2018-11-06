#include "cliente/video/administrador_texturas.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "cliente/video/error_sdl.h"
#include "cliente/video/log.h"
#include "cliente/video/textura.h"

namespace cliente {

AdministradorTexturas::AdministradorTexturas(SDL_Renderer *renderer_) {
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        throw ErrorSDL("IMG_Init", IMG_GetError());
    renderer = renderer_;
    fuente = TTF_OpenFont("../data/PatrickHand-Regular.ttf", 16);
    if (!fuente)
        throw ErrorSDL("TTF_OpenFont", TTF_GetError());
}

const Textura& AdministradorTexturas::cargar_imagen(const char *img) {
    if (texturas.count(img) != 0)
        return texturas.find(img)->second;
    
    SDL_Surface *superficie = IMG_Load(img); // SDL_LoadBMP(img);
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

bool AdministradorTexturas::contiene_textura(const std::string& id) {
    return texturas_creadas.find(id) != texturas_creadas.end();
}

Textura& AdministradorTexturas::obtener_textura(const std::string& id) {
    return texturas_creadas.find(id)->second;
}

Textura& AdministradorTexturas::crear_textura(const std::string& id, int w, 
    int h) 
{ 
    SDL_Texture *textura = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, w, h);
    
    if (!textura)
        throw ErrorSDL("SDL_CreateTexture");
    
    if (texturas_creadas.find(id) != texturas_creadas.end())
        throw std::runtime_error("Ya hay una textura creada con ese id");

    texturas_creadas.emplace(id, Textura(renderer, textura));
    
    return texturas_creadas.find(id)->second;
}

Textura AdministradorTexturas::crear_texto(const std::string& texto) {
    SDL_Surface *sf = TTF_RenderUTF8_Blended(fuente, texto.c_str(), {255, 255, 255, 0});
    if (!sf)
        throw ErrorSDL("TTF_RenderUTF8_Blended", TTF_GetError());
    
    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderer, sf);
    SDL_FreeSurface(sf);

    if (!textura)
        throw ErrorSDL("SDL_CreateTextureFromSurface");
    
    return Textura(renderer, textura);
}

AdministradorTexturas::~AdministradorTexturas() {
    IMG_Quit();
}

} // namespace cliente
