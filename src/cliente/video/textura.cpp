#include "cliente/video/textura.h"

#include "cliente/video/error_sdl.h"

namespace cliente {

Textura::Textura(SDL_Renderer *renderer_, SDL_Texture* textura_) {
    renderer = renderer_;
    textura = textura_;
    src.x = src.y = 0;
    if (SDL_QueryTexture(textura, NULL, NULL, &src.w, &src.h) != 0)
        throw ErrorSDL("SDL_QueryTexture");
}

Textura::Textura() {
    textura = NULL;
    renderer = NULL;
    src = { 0, 0, 0, 0 };
}

Textura::Textura(Textura&& otro) {
    textura = otro.textura;
    renderer = otro.renderer;
    src = otro.src;
    otro.textura = NULL;
    otro.renderer = NULL;
    otro.src = { 0, 0, 0, 0 };
}

Textura& Textura::operator=(Textura&& otro) {
    if (this == &otro)
        return *this;
    
    if (textura)
        SDL_DestroyTexture(textura);
    
    textura = otro.textura;
    renderer = otro.renderer;
    src = otro.src;
    otro.textura = NULL;
    otro.renderer = NULL;
    otro.src = { 0, 0, 0, 0 };
    return *this;
}

int Textura::obtener_alto() const {
    return src.h;
}

int Textura::obtener_ancho() const {
    return src.w;
}

void Textura::renderizar(int x, int y) const {
    SDL_Rect dst;
    dst.x = x; dst.y = y;
    dst.w = src.w; dst.h = src.h;

    if (SDL_RenderCopy(renderer, textura, NULL, &dst) != 0)
        throw ErrorSDL("SDL_RenderCopy");
}

void Textura::renderizar(int x, int y, Textura& otro) const {
    SDL_Texture *old_target = SDL_GetRenderTarget(renderer);
    if (SDL_SetRenderTarget(renderer, otro.textura) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
    
    SDL_Rect dst;
    dst.x = x; dst.y = y;
    dst.w = src.w; dst.h = src.h;

    if (SDL_RenderCopy(renderer, textura, NULL, &dst) != 0)
        throw ErrorSDL("SDL_RenderCopy");
    
    if (SDL_SetRenderTarget(renderer, old_target) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
}

void Textura::renderizar(int x, int y, const Rectangulo& seccion) const {
    SDL_Rect dst;
    dst.x = x; dst.y = y;
    dst.w = seccion.sdl_rect.w; dst.h = seccion.sdl_rect.h;

    if (SDL_RenderCopy(renderer, textura, &seccion.sdl_rect, &dst) != 0)
        throw ErrorSDL("SDL_RenderCopy");
}

void Textura::renderizar(int x, int y, const Rectangulo& seccion, 
    Textura& otro) const {
    SDL_Texture *old_target = SDL_GetRenderTarget(renderer);
    if (SDL_SetRenderTarget(renderer, otro.textura) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
    
    SDL_Rect dst;
    dst.x = x; dst.y = y;
    dst.w = seccion.sdl_rect.w; dst.h = seccion.sdl_rect.h;

    if (SDL_RenderCopy(renderer, textura, &seccion.sdl_rect, &dst) != 0)
        throw ErrorSDL("SDL_RenderCopy");
    
    if (SDL_SetRenderTarget(renderer, old_target) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
}

} // namespace cliente
