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
    fuente = TTF_OpenFont("./assets/fuente.ttf", 16);
    if (!fuente)
        throw ErrorSDL("TTF_OpenFont", TTF_GetError());
}

const Textura& AdministradorTexturas::cargar_imagen(const std::string& img) {
    if (texturas.count(img) != 0)
        return texturas.find(img)->second;

    SDL_Texture *textura;

    if ((img.length() > 3) && (img.substr(img.length() - 3) == "bmp")) {
        SDL_Surface *superficie = IMG_Load(img.c_str());
        if (!superficie)
            throw ErrorSDL("IMG_Load", IMG_GetError());
        
        SDL_SetColorKey(superficie, SDL_TRUE, 0);
        textura = SDL_CreateTextureFromSurface(renderer, superficie);
        SDL_FreeSurface(superficie);
    } else {
        // Los PNG ya tienen su alpha
        textura = IMG_LoadTexture(renderer, img.c_str());
    }

    if (!textura)
        throw ErrorSDL("SDL_CreateTextureFromSurface");

    texturas.emplace(img, Textura(renderer, textura));
    
    log_depuracion("Cargado %s", img.c_str());
    return texturas.find(img)->second;
}

bool AdministradorTexturas::contiene_textura(const std::string& id) {
    return texturas_creadas.find(id) != texturas_creadas.end();
}

Textura& AdministradorTexturas::obtener_textura(const std::string& id) {
    return texturas_creadas.find(id)->second;
}

Textura AdministradorTexturas::eliminar_textura(const std::string& id) {
    Textura eliminada = std::move(texturas_creadas.find(id)->second);
    texturas_creadas.erase(id);
    return eliminada;
}

Textura& AdministradorTexturas::crear_textura(const std::string& id, int w, 
    int h) 
{ 
    if (texturas_creadas.find(id) != texturas_creadas.end())
        throw std::runtime_error("Ya hay una textura creada con ese id");
    
    SDL_Texture *textura = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, w, h);
    
    if (!textura)
        throw ErrorSDL("SDL_CreateTexture");

    texturas_creadas.emplace(id, Textura(renderer, textura));
    
    return texturas_creadas.find(id)->second;
}

Textura& AdministradorTexturas::crear_texto(const std::string& texto) {
    if (texturas_creadas.find("texto-" + texto) != texturas_creadas.end())
        return texturas_creadas.find("texto-" + texto)->second;

    SDL_Surface *sf = TTF_RenderUTF8_Blended(fuente, texto.c_str(), {255, 255, 255, 0});
    if (!sf)
        throw ErrorSDL("TTF_RenderUTF8_Blended", TTF_GetError());
    
    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderer, sf);
    SDL_FreeSurface(sf);

    if (!textura)
        throw ErrorSDL("SDL_CreateTextureFromSurface");
    
    texturas_creadas.emplace("texto-" + texto, Textura(renderer, textura));
    return texturas_creadas.find("texto-" + texto)->second;
}

Textura AdministradorTexturas::crear_texto_nc(const std::string& texto) {
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
    TTF_CloseFont(fuente);
    IMG_Quit();
}

} // namespace cliente
