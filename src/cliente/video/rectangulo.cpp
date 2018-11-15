#include "cliente/video/rectangulo.h"

#include "cliente/video/posicion.h"

namespace cliente {

Rectangulo::Rectangulo(int x, int y, int ancho, int alto) {
    sdl_rect.x = x;
    sdl_rect.y = y;
    sdl_rect.w = ancho;
    sdl_rect.h = alto;
    esq_sup_izq = Posicion(x, y);
    esq_inf_der = Posicion(x + ancho, y + alto);
    w = ancho; h = alto;
}

Rectangulo::Rectangulo(const Posicion& esquina_sup_izq, 
    const Posicion& esquina_inf_der)
{
    esq_inf_der = esquina_inf_der;
    esq_sup_izq = esquina_sup_izq;
    w = esq_inf_der.x - esq_sup_izq.x;
    h = esq_inf_der.y - esq_sup_izq.y;
    sdl_rect.x = esq_sup_izq.x;
    sdl_rect.y = esq_sup_izq.y;
    sdl_rect.w = w;
    sdl_rect.h = h;
}

} // namespace cliente
