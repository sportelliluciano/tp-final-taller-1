#include "cliente/modelo/hud/fps.h"

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

void FPS::renderizar(Ventana& ventana, int x, int y) {
    std::stringstream s_fps;
    s_fps << "FPS: " << ventana.fps();

    Textura& textura_fps = ventana
        .obtener_administrador_texturas()
        .crear_texto(s_fps.str());
    
    textura_fps.renderizar(x + PADDING_FPS_X, y + PADDING_FPS_Y);
}

} // namespace cliente
