#include "cliente/modelo/hud/barra_superior/fps.h"

#include <sstream>
#include <string>

#include "cliente/video/ventana.h"

#define ANCHO_FPS 70
#define ALTO_FPS 30

#define PADDING_FPS_X 10
#define PADDING_FPS_Y 2

namespace cliente {

int FPS::obtener_alto() const {
    return ALTO_FPS;
}

int FPS::obtener_ancho() const {
    return ANCHO_FPS;
}

void FPS::renderizar(Ventana& ventana, const Posicion& punto) {
    if (last_fps != ventana.fps()) {
        std::stringstream s_fps;
        s_fps << "FPS: " << ventana.fps();
        last_fps = ventana.fps();
        s_last_fps = s_fps.str();
    }

    Textura& textura_fps = ventana
        .obtener_administrador_texturas()
        .crear_texto(s_last_fps);
    
    textura_fps.renderizar(punto.x + PADDING_FPS_X, punto.y + PADDING_FPS_Y);
}

} // namespace cliente
