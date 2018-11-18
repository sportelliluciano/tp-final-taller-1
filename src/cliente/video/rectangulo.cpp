#include "cliente/video/rectangulo.h"

#include "cliente/video/posicion.h"

#define min(a, b) (((a) < (b)) ? (a):(b))

namespace cliente {

Rectangulo::Rectangulo(int x, int y, int ancho, int alto) {
    sdl_rect.x = x;
    sdl_rect.y = y;
    sdl_rect.w = ancho;
    sdl_rect.h = alto;
}

Rectangulo::Rectangulo(const Posicion& esquina_a, 
    const Posicion& esquina_b)
{
    sdl_rect.w = abs(esquina_a.x - esquina_b.x);
    sdl_rect.h = abs(esquina_a.y - esquina_b.y);
    sdl_rect.x = min(esquina_a.x, esquina_b.x);
    sdl_rect.y = min(esquina_a.y, esquina_b.y);
}

bool Rectangulo::contiene_punto(const Posicion& punto) const {
    return ((sdl_rect.x <= punto.x) && (punto.x <= sdl_rect.x + sdl_rect.w)) &&
           ((sdl_rect.y <= punto.y) && (punto.y <= sdl_rect.y + sdl_rect.h));
}

int Rectangulo::x() const {
    return sdl_rect.x;
}

void Rectangulo::x(int nuevo_x) {
    sdl_rect.x = nuevo_x;
}


int Rectangulo::y() const {
    return sdl_rect.y;
}

void Rectangulo::y(int nuevo_y) {
    sdl_rect.y = nuevo_y;
}


int Rectangulo::ancho() const {
    return sdl_rect.w;
}

void Rectangulo::ancho(int nuevo_ancho) {
    sdl_rect.w = nuevo_ancho;
}


int Rectangulo::alto() const {
    return sdl_rect.h;
}

void Rectangulo::alto(int nuevo_alto) {
    sdl_rect.h = nuevo_alto;
}


Posicion Rectangulo::esquina_sup_izq() const {
    return Posicion(sdl_rect.x, sdl_rect.y);
}

void Rectangulo::esquina_sup_izq(const Posicion& nueva_esquina_sup_izq) {
    sdl_rect.x = nueva_esquina_sup_izq.x;
    sdl_rect.y = nueva_esquina_sup_izq.y;
}


Posicion Rectangulo::esquina_inf_der() const {
    return Posicion(sdl_rect.x + sdl_rect.w, sdl_rect.y + sdl_rect.h);
}

void Rectangulo::esquina_inf_der(const Posicion& nueva_esquina_inf_der) {
    sdl_rect.w = abs(sdl_rect.x - nueva_esquina_inf_der.x);
    sdl_rect.h = abs(sdl_rect.y - nueva_esquina_inf_der.y);
}


const SDL_Rect& Rectangulo::rect() const {
    return sdl_rect;
} 

void Rectangulo::rect(const SDL_Rect& nuevo_rect) {
    sdl_rect = nuevo_rect;
}


} // namespace cliente
