#include "cliente/modelo/hud/boton.h"

#include "cliente/modelo/sprite.h"
#include "cliente/video/ventana.h"

#define ANCHO_DEF 60
#define ALTO_DEF 50

namespace cliente {

Boton::Boton()
    : sprite(0), boton(0, 0, ANCHO_DEF, ALTO_DEF), imagen(nullptr)
{ }

Boton::Boton(int sprite_id)
    : sprite(sprite_id), boton(0, 0, ANCHO_DEF, ALTO_DEF)
{ }

void Boton::set_sprite(int sprite_id) {
    sprite = sprite_id;
    imagen = nullptr;
}

void Boton::set_imagen(const char *img) {
    sprite = -1;
    imagen = img;
}

void Boton::registrar_click(std::function<void(void)> callback) {
    cb_click = callback;
}

void Boton::click() {
    if (cb_click)
        cb_click();
}

void Boton::set_tamanio(int ancho, int alto) {
    boton = Rectangulo(0, 0, ancho, alto);
}

void Boton::autoresize(bool activar) {
    autoresize_activo = activar;
}

void Boton::renderizar(Ventana& ventana, int x, int y) {
    if (sprite != -1) {
        const Textura& t = Sprite(sprite).obtener_textura(ventana);
        if (autoresize_activo)
            boton = t.obtener_rect();
        t.renderizar(x, y, boton, t.obtener_rect());
    } else if (imagen != nullptr) {
        const Textura& t = 
            ventana.obtener_administrador_texturas().cargar_imagen(imagen);
        if (autoresize_activo)
            boton = t.obtener_rect();
        t.renderizar(x, y, boton, t.obtener_rect());
    }
}

int Boton::obtener_alto() const {
    return boton.sdl_rect.h;
}

int Boton::obtener_ancho() const {
    return boton.sdl_rect.w;
}

} // namespace cliente
