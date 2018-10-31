#include "cliente/modelo/hud.h"

#include <string>
#include <sstream>

#include "cliente/video/ventana.h"

namespace cliente {

HUD::HUD() {

}

void HUD::renderizar(Ventana& ventana) {
    std::stringstream s_fps;
    s_fps << "FPS: " << ventana.fps();

    Textura textura_fps = ventana
        .obtener_administrador_texturas()
        .crear_texto(s_fps.str());
    
    textura_fps.renderizar(ventana.ancho() - textura_fps.obtener_ancho() - 10,
        10);
}

} // namespace cliente
