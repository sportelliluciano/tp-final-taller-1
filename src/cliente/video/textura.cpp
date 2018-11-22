#include "cliente/video/textura.h"

#include "cliente/video/error_sdl.h"
#include "cliente/video/log.h"

namespace cliente {

Textura::Textura(SDL_Renderer *renderer_, SDL_Texture* textura_) {
    renderer = renderer_;
    textura = textura_;
    src.x = src.y = 0;
    if (SDL_QueryTexture(textura, NULL, NULL, &src.w, &src.h) != 0)
        throw ErrorSDL("SDL_QueryTexture");
    SDL_SetTextureBlendMode(textura, SDL_BLENDMODE_BLEND);
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
    dst.w = seccion.ancho(); dst.h = seccion.alto();

    if (SDL_RenderCopy(renderer, textura, &seccion.rect(), &dst) != 0)
        throw ErrorSDL("SDL_RenderCopy");
}

void Textura::renderizar(int x, int y, const Rectangulo& seccion, 
    const Rectangulo& destino) const 
{
    SDL_Rect dst = destino.rect();
    dst.x = x; dst.y = y;
    if (SDL_RenderCopy(renderer, textura, &seccion.rect(), &dst) != 0) {
        throw ErrorSDL("SDL_RenderCopy");
    }
}

void Textura::renderizar(int x, int y, const Rectangulo& seccion, 
    Textura& otro) const {
    SDL_Texture *old_target = SDL_GetRenderTarget(renderer);
    if (SDL_SetRenderTarget(renderer, otro.textura) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
    
    SDL_Rect dst;
    dst.x = x; dst.y = y;
    dst.w = seccion.ancho(); dst.h = seccion.alto();

    if (SDL_RenderCopy(renderer, textura, &seccion.rect(), &dst) != 0)
        throw ErrorSDL("SDL_RenderCopy");
    
    if (SDL_SetRenderTarget(renderer, old_target) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
}

Rectangulo Textura::obtener_rect() const {
    return Rectangulo(0, 0, src.w, src.h);
}

void Textura::limpiar(int r, int g, int b, int a) {
    SDL_Texture *old_target = SDL_GetRenderTarget(renderer);
    if (SDL_SetRenderTarget(renderer, textura) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
    
    Uint8 rr, gg, bb, aa;
    SDL_GetRenderDrawColor(renderer, &rr, &gg, &bb, &aa);
    SDL_SetRenderDrawColor(renderer, (Uint8)r, (Uint8)g, (Uint8)b, (Uint8) a);
    SDL_RenderFillRect(renderer, &src);
    SDL_SetRenderDrawColor(renderer, rr, gg, bb, aa);
    
    if (SDL_SetRenderTarget(renderer, old_target) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
}

void Textura::dibujar_rectangulo(const Rectangulo& rc, int ancho_linea, 
    Uint8 r, Uint8 g, Uint8 b, Uint8 a) 
{
    SDL_Texture *old_target = SDL_GetRenderTarget(renderer);
    if (SDL_SetRenderTarget(renderer, textura) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
    
    Uint8 rr, gg, bb, aa;
    SDL_GetRenderDrawColor(renderer, &rr, &gg, &bb, &aa);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    Rectangulo linea = Rectangulo(rc.x() + ancho_linea, rc.y(), rc.ancho(), 
        ancho_linea);
    SDL_RenderFillRect(renderer, &linea.rect());

    linea.y(rc.y() + rc.alto());
    SDL_RenderFillRect(renderer, &linea.rect());

    linea.x(rc.x());
    linea.y(rc.y());
    linea.ancho(ancho_linea);
    linea.alto(rc.alto() + ancho_linea);
    SDL_RenderFillRect(renderer, &linea.rect());

    linea.x(rc.x() + rc.ancho());
    SDL_RenderFillRect(renderer, &linea.rect());
    SDL_SetRenderDrawColor(renderer, rr, gg, bb, aa);
    if (SDL_SetRenderTarget(renderer, old_target) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
}

void Textura::rellenar_rectangulo(const Rectangulo& rc,Uint8 r, Uint8 g, 
    Uint8 b, Uint8 a)
{
    SDL_Texture *old_target = SDL_GetRenderTarget(renderer);
    if (SDL_SetRenderTarget(renderer, textura) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
    
    Uint8 rr, gg, bb, aa;
    SDL_GetRenderDrawColor(renderer, &rr, &gg, &bb, &aa);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    SDL_RenderFillRect(renderer, &rc.rect());

    SDL_SetRenderDrawColor(renderer, rr, gg, bb, aa);
    if (SDL_SetRenderTarget(renderer, old_target) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
}

Textura::~Textura() {
    if (textura)
        SDL_DestroyTexture(textura);

    textura = nullptr;
}

} // namespace cliente
