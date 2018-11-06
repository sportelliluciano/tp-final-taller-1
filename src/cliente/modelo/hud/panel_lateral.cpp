#include "cliente/modelo/hud/panel_lateral.h"

#include <iostream>

#include "cliente/modelo/hud/botonera.h"
#include "cliente/modelo/juego.h"

#define ANCHO_PANEL_LATERAL 250

namespace cliente {

PanelLateral::PanelLateral(Juego& juego_)
: botonera(ANCHO_PANEL_LATERAL, 300), juego(juego_)
{
    std::vector<const Edificio*> edificios = juego.obtener_edificios();

    for (auto it=edificios.begin(); it != edificios.end(); ++it) {
        const Edificio& edificio = **it;
        Boton *btn = botonera.crear_boton();
        btn->set_sprite(edificio.obtener_sprite_boton());
        btn->registrar_click(
            [this, edificio]() {
                if (edificio.obtener_id() == "trampas_aire") {
                    std::cout << "beatingggg" << std::endl;
                    sonido.iniciar_musica_fondo(MUSICA_0);
                } else {
                    std::cout << "Construir edificio" << std::endl;
                    sonido.reproducir_sonido(SONIDO_BLEEP, 100);
                    juego.iniciar_construccion(edificio.obtener_id());
                }
            }
        );
    }
}

bool PanelLateral::contiene_punto(int x, int y) const {
    return (base_x <= x);
}

void PanelLateral::click_derecho(int x, int y) {

}

void PanelLateral::click_izquierdo(int x, int y) {
    std::cout << "Click en panel lateral" << std::endl;
    if (y - base_botonera_y > 0) {
        std::cout << "Click en botonera" << std::endl;
        botonera.click_izquierdo(x - base_x, y - base_botonera_y);
    }
}

void PanelLateral::renderizar(Ventana& ventana, const Juego&) {
    AdministradorTexturas& almacen = ventana.obtener_administrador_texturas();
    if (!almacen.contiene_textura("panel-lateral")) {
        ventana
            .obtener_administrador_texturas()
            .crear_textura("panel-lateral", ANCHO_PANEL_LATERAL, 
                ventana.alto());
    }

    ventana
        .obtener_administrador_texturas()
        .obtener_textura("panel-lateral")
        .renderizar(ventana.ancho() - ANCHO_PANEL_LATERAL, 0);
    
    botonera.renderizar(ventana, ventana.ancho() - ANCHO_PANEL_LATERAL, 
        ventana.alto() - 300);
    
    // TODO: revisar esto
    base_x = ventana.ancho() - ANCHO_PANEL_LATERAL;
    base_botonera_y = ventana.alto() - 300;
}

} // namespace cliente
