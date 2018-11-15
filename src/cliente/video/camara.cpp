#include "cliente/video/camara.h"

#include "cliente/video/posicion.h"
#include "cliente/video/rectangulo.h"

namespace cliente {

Camara::Camara() : ancho(0), alto(0), cam_x(0), cam_y(0) { }

Camara::Camara(int ancho_, int alto_) 
    : ancho(ancho_), alto(alto_), cam_x(0), cam_y(0)
{ }

void Camara::mover_camara(const Posicion& posicion_nueva) {
    cam_x = posicion_nueva.x;
    cam_y = posicion_nueva.y;
}

void Camara::desplazar_camara(const Posicion& delta) {
    cam_x += delta.x;
    cam_y += delta.y;
}

Posicion Camara::traducir_a_visual(const Posicion& posicion_logica) {
    return Posicion(posicion_logica.x - cam_x, posicion_logica.y - cam_y);
}

Posicion Camara::traducir_a_logica(const Posicion& posicion_visual) {
    return Posicion(posicion_visual.x + cam_x, posicion_visual.y + cam_y);
}

bool Camara::es_visible(const Rectangulo& area) {
    return (abs(area.esq_sup_izq.x - cam_x) * 2 < (area.w + ancho)) &&
         (abs(area.esq_sup_izq.y - cam_y) * 2 < (area.h + alto));
}

Rectangulo Camara::obtener_vista() {
    return Rectangulo(cam_x, cam_y, ancho, alto);
}

} // namespace cliente
