#include "cliente/modelo/hud/boton.h"

#include "cliente/modelo/sprite.h"
#include "cliente/video/ventana.h"

#define ANCHO_DEF 60
#define ALTO_DEF 50

namespace cliente {

Boton::Boton()
: imagen(nullptr), boton(0, 0, ANCHO_DEF, ALTO_DEF)
{ }

void Boton::set_imagen(const char *img) {
    imagen = img;
}

void Boton::set_tamanio(int ancho, int alto) {
    boton = Rectangulo(0, 0, ancho, alto);
}

void Boton::set_padding(int x, int y) {
    padding_x = x;
    padding_y = y;
}

void Boton::centrar_imagen(bool activar) {
    autopadding = activar;
}

void Boton::registrar_click(std::function<void(void)> callback) {
    cb_click = callback;
}

int Boton::obtener_alto() const {
    return boton.alto();
}

int Boton::obtener_ancho() const {
    return boton.ancho();
}

static Rectangulo escalar_rectangulo(const Rectangulo& original, 
    const Rectangulo& destino) 
{
    Rectangulo escalado = destino;

    float relacion_original = original.alto() / (float)original.ancho();
    float relacion_destino = destino.alto() / (float)destino.ancho();
    
    if (relacion_destino < relacion_original) {
        // 2x1 < 1x2
        //  __   _
        // |__| | |
        //      |_|
        escalado.ancho(original.ancho() * (destino.alto() / 
            (float)original.alto()));
    } else {
        escalado.alto(original.alto() * (destino.ancho() / 
            (float)original.ancho()));
    }

    return escalado;
}

void Boton::renderizar(Ventana& ventana, const Posicion& punto) {
    if (!imagen)
        return;
    
    const Textura& textura_imagen = 
        ventana.obtener_administrador_texturas().cargar_imagen(imagen);

    Rectangulo destino = escalar_rectangulo(textura_imagen.obtener_rect(), 
        boton);

    if (autopadding) {
        padding_x = (boton.ancho() - destino.ancho()) / 2;
        padding_y = (boton.alto() - destino.alto()) / 2;
    }

    textura_imagen.renderizar(punto.x + padding_x, punto.y + padding_y, 
        textura_imagen.obtener_rect(), destino);
}

bool Boton::mouse_click_izquierdo(const Posicion&) {
    if (cb_click)
        cb_click();
    return false;
}

} // namespace cliente
