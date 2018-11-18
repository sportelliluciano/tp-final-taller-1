#include "cliente/modelo/hud/boton.h"

#include <iostream>

#include "cliente/modelo/sprite.h"
#include "cliente/video/ventana.h"

#define ANCHO_DEF 60
#define ALTO_DEF 50

namespace cliente {

Boton::Boton()
    : imagen(nullptr), sprite(0), boton(0, 0, ANCHO_DEF, ALTO_DEF)
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
        
        Rectangulo seccion(0, 0, t.obtener_ancho(), t.obtener_alto());
        Rectangulo destino = boton;

        int h_boton = boton.alto();
        int w_boton = boton.ancho();
        float relacion_boton = h_boton / (1.0f * w_boton);
        int h_img = t.obtener_ancho();
        int w_img = t.obtener_alto();
        float relacion_img = h_img / (1.0f * w_img);

        if (relacion_boton < relacion_img) {
            // 2x1 < 1x2
            //  __   _
            // |__| | |
            //      |_|
            w_img = w_img * (h_boton / (1.0f * h_img));
            h_img = h_boton;
        } else {
            h_img = h_img * (w_boton / (1.0f * w_img));
            w_img = w_boton;
        }

        destino.ancho(w_img);
        destino.alto(h_img);

        if (autopadding) {
            padding_x = (boton.ancho() - w_img) / 2;
            padding_y = (boton.alto() - h_img) / 2;
        }

        t.renderizar(x + padding_x, y + padding_y, seccion, destino);
    }
}

void Boton::set_autopadding(bool activar) {
    autopadding = activar;
}

void Boton::set_padding(int x, int y) {
    padding_x = x;
    padding_y = y;
}

int Boton::obtener_alto() const {
    return boton.alto();
}

int Boton::obtener_ancho() const {
    return boton.ancho();
}

bool Boton::mouse_click_izquierdo(int x, int y) {
    std::cout << "Boton: click izq en (" << x << "," << y << ")" << std::endl;
    if (cb_click)
        cb_click();
    return false;
}

} // namespace cliente
