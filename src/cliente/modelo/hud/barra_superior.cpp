#include "cliente/modelo/hud/barra_superior.h"

#include <string>
#include <sstream>

#include "cliente/modelo/juego.h"
#include "cliente/video/ventana.h"

#define ALTO_BARRA 40
#define ANCHO_BOTONERA 72

namespace cliente {

BarraSuperior::BarraSuperior(int ancho_ventana)
    : botonera(ANCHO_BOTONERA, ALTO_BARRA) 
{
    ancho = ancho_ventana;
    Boton *musica = botonera.crear_boton();
    Boton *sonido = botonera.crear_boton();

    musica->set_imagen("./assets/nuevos/musica-deshabilitada.png");
    musica->autoresize(true);
    sonido->set_imagen("./assets/nuevos/con-sonido.png");
    sonido->autoresize(true);

    musica->registrar_click(
        [musica] () {
            musica->set_imagen("./assets/nuevos/musica-habilitada.png");
        }
    );

    sonido->registrar_click(
        [sonido] () {
            sonido->set_imagen("./assets/nuevos/sin-sonido.png");
        }
    );
}

bool BarraSuperior::contiene_punto(int x, int y) const {
    return ((0 <= x) && (x <= ancho)) && ((0 <= y) && (y <= ALTO_BARRA));
}

void BarraSuperior::click_derecho(int x, int y) {

}

void BarraSuperior::click_izquierdo(int x, int y) {
    botonera.click_izquierdo(x, y);
}


void BarraSuperior::renderizar(Ventana& ventana, const Juego& juego) {
    AdministradorTexturas& almacen = ventana.obtener_administrador_texturas();
    if (!almacen.contiene_textura("barra-superior")) {
        ventana
            .obtener_administrador_texturas()
            .crear_textura("barra-superior", ventana.ancho(), ALTO_BARRA);
    }

    ventana
        .obtener_administrador_texturas()
        .obtener_textura("barra-superior")
        .renderizar(0, 0);
    
    std::stringstream s_fps;
    s_fps << "FPS: " << ventana.fps();

    Textura textura_fps = ventana
        .obtener_administrador_texturas()
        .crear_texto(s_fps.str());
    
    textura_fps.renderizar(ventana.ancho() - textura_fps.obtener_ancho() - 10,
        10);
    
    dinero.setear_dinero(juego.obtener_dinero());
    dinero.renderizar(ventana, 800, 15);
    botonera.renderizar(ventana, 0, 0);
}

} // namespace cliente
