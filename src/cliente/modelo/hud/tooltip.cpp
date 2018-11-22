#include "cliente/modelo/hud/tooltip.h"

#include <string>
#include <sstream>

#include "cliente/video/ventana.h"

#define ANCHO_TOOLTIP 200
#define ALTO_TOOLTIP 120

namespace cliente {

void Tooltip::set_titulo(const std::string& titulo) {
    titulo_ = titulo;
}

void Tooltip::set_cuerpo(const std::string& cuerpo) {
    cuerpo_ = cuerpo;
}

void Tooltip::set_pie(const std::string& pie) {
    pie_ = pie;
}

void Tooltip::renderizar(Ventana& ventana, int x, int y) {
    Textura& titulo = ventana
        .obtener_administrador_texturas()
        .crear_texto(titulo_, Rectangulo(0, 0, ANCHO_TOOLTIP, ANCHO_TOOLTIP),1);
        
    Textura& cuerpo = ventana
        .obtener_administrador_texturas()
        .crear_texto(cuerpo_, Rectangulo(0, 0, ANCHO_TOOLTIP, ANCHO_TOOLTIP),0);
    
    Textura& pie = ventana
        .obtener_administrador_texturas()
        .crear_texto(pie_, Rectangulo(0, 0, ANCHO_TOOLTIP, ANCHO_TOOLTIP), 0);
    
    Textura& tooltip = ventana
        .obtener_administrador_texturas()
        .obtener_o_crear_textura("tooltip", ANCHO_TOOLTIP, ALTO_TOOLTIP);
    
    y -= ALTO_TOOLTIP;
    if (x + ANCHO_TOOLTIP > ventana.ancho())
        x = ventana.ancho() - ANCHO_TOOLTIP;
    tooltip.limpiar(0, 0, 0, 128);
    tooltip.renderizar(x, y);
    titulo.renderizar(x, y);
    cuerpo.renderizar(x, y + titulo.obtener_alto());
    pie.renderizar(x, y + titulo.obtener_alto() + cuerpo.obtener_alto());
}

} // namespace cliente