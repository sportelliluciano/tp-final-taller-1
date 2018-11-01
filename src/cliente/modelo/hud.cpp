#include "cliente/modelo/hud.h"

#include <string>
#include <sstream>

#include "cliente/modelo/juego.h"
#include "cliente/video/ventana.h"

namespace cliente {

HUD::HUD() : btn(4321, 800, 400) {

}

void HUD::renderizar(Ventana& ventana, const Juego& juego) {
    std::stringstream s_fps;
    s_fps << "FPS: " << ventana.fps();

    Textura textura_fps = ventana
        .obtener_administrador_texturas()
        .crear_texto(s_fps.str());
    
    textura_fps.renderizar(ventana.ancho() - textura_fps.obtener_ancho() - 10,
        10);
    
    btn.renderizar(ventana);
    dinero.setear_dinero(juego.obtener_dinero());
    dinero.renderizar(ventana, 800, 30);
}

} // namespace cliente
