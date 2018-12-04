#include "cliente/video/textura.h"

#include "cliente/video/error_sdl.h"
#include "comun/log.h"

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

/**
 * \brief Cambia el color del renderer y devuelve el color anterior.
 */
static Color set_render_color(SDL_Renderer *renderer, const Color& color) {
    SDL_Color anterior;
    const SDL_Color* nuevo = color.obtener_color();
    SDL_GetRenderDrawColor(renderer, &anterior.r, &anterior.g, &anterior.b, 
        &anterior.a);
    SDL_SetRenderDrawColor(renderer, nuevo->r, nuevo->g, nuevo->b, nuevo->a);
    
    return Color(&anterior);
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

void Textura::renderizar(int x, int y, const Rectangulo& seccion, 
    const Rectangulo& destino, Textura& renderizable) const
{
    SDL_Texture *old_target = SDL_GetRenderTarget(renderer);
    if (SDL_SetRenderTarget(renderer, renderizable.textura) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
    
    SDL_Rect dst;
    dst.x = x; dst.y = y;
    dst.w = destino.ancho(); dst.h = destino.alto();


    if (SDL_RenderCopy(renderer, textura, &seccion.rect(), &dst) != 0)
        throw ErrorSDL("SDL_RenderCopy");
    
    if (SDL_SetRenderTarget(renderer, old_target) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");   
}

Rectangulo Textura::obtener_rect() const {
    return Rectangulo(0, 0, src.w, src.h);
}

void Textura::limpiar() {
    limpiar(Color(0, 0, 0, 0));
}

void Textura::limpiar(const Color& color) {
    SDL_Texture *old_target = SDL_GetRenderTarget(renderer);
    if (SDL_SetRenderTarget(renderer, textura) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
    
    Color anterior = set_render_color(renderer, color);
    SDL_RenderClear(renderer);
    set_render_color(renderer, anterior);
    
    if (SDL_SetRenderTarget(renderer, old_target) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
}

void Textura::dibujar_rectangulo(const Rectangulo& rc, int ancho_linea, 
    const Color& color) 
{
    SDL_Texture *old_target = SDL_GetRenderTarget(renderer);
    if (SDL_SetRenderTarget(renderer, textura) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
    
    Color anterior = set_render_color(renderer, color);

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
    
    set_render_color(renderer, anterior);
    
    if (SDL_SetRenderTarget(renderer, old_target) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
}

void Textura::rellenar_rectangulo(const Rectangulo& rc, const Color& color) {
    SDL_Texture *old_target = SDL_GetRenderTarget(renderer);
    if (SDL_SetRenderTarget(renderer, textura) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
    
    Color anterior = set_render_color(renderer, color);

    SDL_RenderFillRect(renderer, &rc.rect());

    set_render_color(renderer, anterior);
    
    if (SDL_SetRenderTarget(renderer, old_target) != 0)
        throw ErrorSDL("SDL_SetRenderTarget");
}

Textura::~Textura() {
    if (textura)
        SDL_DestroyTexture(textura);

    textura = nullptr;
}

} // namespace cliente
