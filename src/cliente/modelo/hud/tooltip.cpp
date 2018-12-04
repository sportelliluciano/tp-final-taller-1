#include "cliente/modelo/hud/tooltip.h"

#include <string>
#include <sstream>

#include "cliente/video/color.h"
#include "cliente/video/ventana.h"

#define ANCHO_TOOLTIP 220
#define ALTO_TOOLTIP 160

#define PADDING_X 5
#define PADDING_Y 5

#define SEPARACION 2

#define MAX_ALTO_CUERPO 75

#define COLOR_FONDO_TOOLTIP Color(0, 0, 0, 225)

namespace cliente {

void Tooltip::set_titulo(const std::string& titulo) {
    titulo_ = titulo;
}

void Tooltip::set_cuerpo(const std::string& cuerpo) {
    cuerpo_ = cuerpo;
}

void Tooltip::set_metadata(
        const std::vector<std::pair<std::string, std::string>>& metadata) 
{
    for (const std::pair<std::string, std::string>& par : metadata) {
        meta.push_back(par.first + ": " + par.second);
    }
}

void Tooltip::set_costo_tiempo(int costo, float tiempo) {
    std::stringstream s;
    s << "Costo: $ " << costo;
    costo_ = s.str();
    
    char buffer[1000];
    snprintf(buffer, 1000, "Tiempo: %g min.", tiempo);
    tiempo_ = buffer;
}

// TODO: caché por acá..
void Tooltip::renderizar(Ventana& ventana, int x, int y) {
    Rectangulo rc(0, 0, ANCHO_TOOLTIP - 2*PADDING_X, 1);
    Textura titulo = ventana
        .obtener_administrador_texturas()
        .crear_texto(titulo_, rc,1);
        
    Textura cuerpo = ventana
        .obtener_administrador_texturas()
        .crear_texto(cuerpo_, rc,0, TAM_FUENTE_CHICO);
    
    Textura costo = ventana
        .obtener_administrador_texturas()
        .crear_texto(costo_, rc,1, TAM_FUENTE_CHICO);
    
    Textura tiempo = ventana
        .obtener_administrador_texturas()
        .crear_texto(tiempo_, rc, 1, TAM_FUENTE_CHICO);
    
    Textura& tooltip = ventana
        .obtener_administrador_texturas()
        .obtener_o_crear_textura("tooltip", ANCHO_TOOLTIP, ALTO_TOOLTIP);
    
    y -= ALTO_TOOLTIP;
    if (x + ANCHO_TOOLTIP > ventana.ancho())
        x = ventana.ancho() - ANCHO_TOOLTIP;
    tooltip.limpiar(COLOR_FONDO_TOOLTIP);
    titulo.renderizar(PADDING_X, PADDING_Y, tooltip);
    int dy = titulo.obtener_alto();
    
    if (cuerpo.obtener_alto() <= MAX_ALTO_CUERPO) {
        cuerpo.renderizar(PADDING_X, PADDING_Y + dy, tooltip);
        dy += cuerpo.obtener_alto();
    } else {
        Textura cuerpo_micro = ventana
            .obtener_administrador_texturas()
            .crear_texto(cuerpo_, rc,0, TAM_FUENTE_MICRO);
        cuerpo_micro.renderizar(PADDING_X, PADDING_Y + dy, tooltip);
        dy += cuerpo_micro.obtener_alto();
    }
    
    dy += SEPARACION;
    bool derecha = true;
    for (const std::string& metaval : meta) {
        Textura m = ventana
            .obtener_administrador_texturas()
            .crear_texto(metaval, Rectangulo(0, 0, ANCHO_TOOLTIP, ANCHO_TOOLTIP), 0, TAM_FUENTE_CHICO);
        
        if (derecha) {
            m.renderizar(PADDING_X, PADDING_Y + dy, tooltip);
        } else {
            m.renderizar(ANCHO_TOOLTIP - PADDING_X - m.obtener_ancho(), 
                PADDING_Y + dy, tooltip);

            dy += m.obtener_alto();
        }

        derecha = !derecha;
    }
    costo.renderizar(PADDING_X, 
        ALTO_TOOLTIP - costo.obtener_alto() - PADDING_Y, tooltip);
    tiempo.renderizar(ANCHO_TOOLTIP - tiempo.obtener_ancho() - PADDING_X, 
        ALTO_TOOLTIP - tiempo.obtener_alto() - PADDING_Y, tooltip);
    
    tooltip.renderizar(x, y);
}

} // namespace cliente