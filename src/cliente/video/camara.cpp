#include "cliente/video/camara.h"

#include "cliente/video/posicion.h"
#include "cliente/video/rectangulo.h"

#define MIN_CAMARA_X -16
#define MIN_CAMARA_Y -16

#define min(x, y) ( ((x) < (y)) ? (x) : (y) )
#define max(x, y) ( ((x) > (y)) ? (x) : (y) )

namespace cliente {

Camara::Camara() : ancho(0), alto(0), cam_x(0), cam_y(0), max_x(0), max_y(0) { }

Camara::Camara(int ancho_, int alto_, int max_x_, int max_y_) 
    : ancho(ancho_), alto(alto_), cam_x(0), cam_y(0), 
      max_x(max_x_), max_y(max_y_)
{ }

void Camara::mover_camara(const Posicion& posicion_nueva) {
    cam_x = min(max(posicion_nueva.x, MIN_CAMARA_X), max_x);
    cam_y = min(max(posicion_nueva.y, MIN_CAMARA_Y), max_y);
}

void Camara::desplazar_camara(const Posicion& delta) {
    mover_camara(Posicion(cam_x + delta.x, cam_y + delta.y));
}

Posicion Camara::traducir_a_visual(const Posicion& posicion_logica) {
    return Posicion(posicion_logica.x - cam_x, posicion_logica.y - cam_y);
}

Posicion Camara::traducir_a_logica(const Posicion& posicion_visual) {
    return Posicion(posicion_visual.x + cam_x, posicion_visual.y + cam_y);
}

bool Camara::es_visible(const Rectangulo& area) {
    return (abs(area.x() - cam_x) * 2 < (area.ancho() + ancho)) &&
         (abs(area.y() - cam_y) * 2 < (area.alto() + alto));
}

Rectangulo Camara::obtener_vista() {
    return Rectangulo(cam_x, cam_y, ancho, alto);
}

} // namespace cliente
