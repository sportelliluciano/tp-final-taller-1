#include "cliente/modelo/controlador.h"

#include <iostream>

#include "cliente/modelo/hud.h"
#include "cliente/modelo/juego.h"
#include "conexion/evento.h"
#include "cliente/servidor.h"
#include "cliente/video/ventana.h"

namespace cliente {

Controlador::Controlador(Ventana& ventana_, Servidor& servidor_, Juego& juego_) 
    : ventana(ventana_), servidor(servidor_), juego(juego_), hud(ventana_, juego_)
{ 
    ventana.registrar_ventana_cerrar([this]() {
        juego.detener();
        servidor.detener();
    });

    ventana.registrar_mouse_click(
        [this](boton_mouse_t boton, int x, int y) { // Click down
            if (boton == BOTON_IZQUIERDO)
                mouse_click_izquierdo(x, y, false);
        },
        [this](boton_mouse_t boton, int x, int y) { // Click up
            if (boton == BOTON_DERECHO)
                mouse_click_derecho(x, y, true);
            else if (boton == BOTON_IZQUIERDO)
                mouse_click_izquierdo(x, y, true);
        }
    );

    ventana.registrar_rueda_mouse(
        [this](int y) {
            mouse_rueda(y);
        }
    );

    ventana.registrar_mouse_motion(
        [this](int x, int y) {
            mouse_x = x;
            mouse_y = y;
        }
    );

    ventana.registrar_teclado(
        [this](tecla_t tecla) { // Teclado down
            recibir_teclado(tecla, false);
        },
        [this](tecla_t tecla) { // Teclado up
            recibir_teclado(tecla, true);
        }
    );
}

void Controlador::actualizar_modelo() {
    conexion::Evento *evento;
    while (servidor.hay_eventos()) {
        evento = servidor.pop_evento();
        evento->actualizar(juego);
        delete evento;
    }
}

void Controlador::procesar_entrada() {

}

void Controlador::mouse_click_derecho(int x, int y, bool up) {
    std::cout << (up ? "UP": "DOWN") << " click derecho en (" << x << ", " << y << ")" << std::endl;
}

void Controlador::mouse_click_izquierdo(int x, int y, bool up) {
    std::cout << (up ? "UP": "DOWN") << " click izquierdo en (" << x << ", " << y << ")" << std::endl;
    if (!up) { // down
        if (!hud.contiene_punto(x, y)) {
            drag_start_x = x;
            drag_start_y = y;
            std::cout << "Drag started" << std::endl;
            draggeando = true;
        }
    } else { // up
        if (draggeando) {
            std::cout << "Drag finished" << std::endl;
            draggeando = false;
            drag_end_x = x;
            drag_end_y = y;
        } else if (hud.contiene_punto(x, y)) {
            hud.click_izquierdo(x, y);
        }
    }
}

void Controlador::recibir_teclado(tecla_t tecla, bool up) {
    std::cout << (up ? "UP":"DOWN") << " " << tecla << std::endl;
}

void Controlador::mouse_rueda(int y) {
    std::cout << "RUEDA > " << y << std::endl;
}

void Controlador::renderizar() {
    if (draggeando) {
        ventana.dibujar_rectangulo(drag_start_x, drag_start_y, mouse_x, mouse_y);
    }

    hud.renderizar(ventana, juego);
}

} // namespace cliente
