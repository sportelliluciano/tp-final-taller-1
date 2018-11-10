#include "cliente/modelo/hud/area_juego.h"

#include <iostream>

#include "cliente/modelo/juego.h"

#define ANCHO_MOV_CAMARA 15
#define ALTO_MOV_CAMARA  15 

/**
 * \brief Delay de la cámara en milisegundos.
 */
#define DELAY_CAMARA_MS 200

namespace cliente {

AreaJuego::AreaJuego(Juego& juego_) 
    : juego(juego_)
{ }
    
int AreaJuego::obtener_alto() const {
    return alto;
}

int AreaJuego::obtener_ancho() const {
    return ancho;
}

void AreaJuego::set_tamanio(int ancho_, int alto_) {
    ancho = ancho_;
    alto = alto_;
}

void AreaJuego::renderizar(Ventana& ventana, int x, int y) {
    ventana.setear_viewport(Rectangulo(x, y, ancho, alto));
    
    if ((mover_camara_x != 0) || (mover_camara_y != 0)) {
        if (ventana.obtener_ms() - delay_camara > DELAY_CAMARA_MS) {
            juego.mover_camara(mover_camara_x, mover_camara_y);
            delay_camara = ventana.obtener_ms();
        }
    }

    juego.renderizar(ventana);

    if (esta_draggeando) {
        ventana.dibujar_rectangulo(drag_start_x, drag_start_y,
            drag_end_x, drag_end_y, 3);
    }
    
    ventana.reestablecer_viewport();
}

bool AreaJuego::mouse_click_izquierdo(int x, int y) {
    juego.seleccionar_edificio(x, y);
    juego.seleccionar_tropas(0, 0, 0, 0);
    return false;
}

bool AreaJuego::mouse_click_derecho(int x, int y) {
    std::cout << "Mover tropas seleccionadas a (" << x << "," << y << ")"
              << std::endl;
    return false;
}

bool AreaJuego::mouse_inicio_arrastre(int x, int y) {
    drag_start_x = x;
    drag_start_y = y;
    esta_draggeando = true;
    return false;
}

bool AreaJuego::mouse_movimiento(int x, int y) {
    if (esta_draggeando) {
        drag_end_x = x;
        drag_end_y = y;
    } else {
        if (x < ANCHO_MOV_CAMARA)
            mover_camara_x = -1;
        else if (x > obtener_ancho() - ANCHO_MOV_CAMARA)
            mover_camara_x = 1;
        else
            mover_camara_x = 0;
        
        if (y < ALTO_MOV_CAMARA)
            mover_camara_y = -1;
        else if (y > obtener_alto() - ALTO_MOV_CAMARA)
            mover_camara_y = 1;
        else
            mover_camara_y = 0;
    }

    return false;
}

bool AreaJuego::mouse_entra(int, int) {
    mover_camara_x = mover_camara_y = 0;
    return false;
}

bool AreaJuego::mouse_sale(int, int) {
    mover_camara_x = mover_camara_y = 0;
    return false;
}


bool AreaJuego::mouse_fin_arrastre(int x, int y) {
    drag_end_x = x;
    drag_end_y = y;
    esta_draggeando = false;
    juego.seleccionar_tropas(drag_start_x, drag_start_y,
        drag_end_x, drag_end_y);
    juego.seleccionar_edificio(-1, -1);
    return false;
}

} // namespace cliente
