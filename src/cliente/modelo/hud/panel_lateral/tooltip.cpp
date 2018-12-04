#include "cliente/modelo/hud/panel_lateral/tooltip.h"

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
#define COLOR_ROSA Color(0xff, 0x73, 0x73, 255)

#define PREFIJO_TOOLTIP "tooltip-"

namespace cliente {

void Tooltip::set_titulo(const std::string& titulo) {
    titulo_ = titulo;
}

void Tooltip::set_cuerpo(const std::string& cuerpo) {
    cuerpo_ = cuerpo;
    id_textura = PREFIJO_TOOLTIP + titulo_ + cuerpo_;
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

void Tooltip::renderizar_metadata(AdministradorTexturas& admin_texturas, 
    Textura& tooltip, int offset_y, const Rectangulo& encuadre) 
{
    bool derecha = true;
    for (const std::string& metaval : meta) {
        Textura m = admin_texturas.crear_texto(metaval, encuadre, COLOR_BLANCO, 
            TAM_FUENTE_CHICO);
        
        if (derecha) {
            m.renderizar(PADDING_X, PADDING_Y + offset_y, tooltip);
        } else {
            m.renderizar(ANCHO_TOOLTIP - PADDING_X - m.obtener_ancho(), 
                PADDING_Y + offset_y, tooltip);

            offset_y += m.obtener_alto();
        }

        derecha = !derecha;
    }
}

Textura Tooltip::crear_textura_cuerpo(AdministradorTexturas& admin_texturas,
    const Rectangulo& encuadre)
{
    Textura cuerpo = admin_texturas.crear_texto(cuerpo_, encuadre, COLOR_BLANCO, 
        TAM_FUENTE_CHICO);
    
    if (cuerpo.obtener_alto() > MAX_ALTO_CUERPO) {
        return admin_texturas.crear_texto(cuerpo_, encuadre, COLOR_BLANCO, 
            TAM_FUENTE_MICRO);
    }

    return cuerpo;
}

Textura& Tooltip::crear_tooltip(AdministradorTexturas& admin_texturas) {
    Textura& tooltip = admin_texturas.crear_textura(id_textura, 
        ANCHO_TOOLTIP, ALTO_TOOLTIP);
    
    Rectangulo encuadre(0, 0, ANCHO_TOOLTIP - (2 * PADDING_X), ALTO_TOOLTIP);
    
    Textura 
        titulo = admin_texturas.crear_texto(titulo_, encuadre, COLOR_ROSA),
        costo = admin_texturas.crear_texto(costo_, encuadre, COLOR_ROSA),
        tiempo = admin_texturas.crear_texto(tiempo_, encuadre, COLOR_ROSA),
        cuerpo = crear_textura_cuerpo(admin_texturas, encuadre);
    
    tooltip.limpiar(COLOR_FONDO_TOOLTIP);

    int dy = titulo.obtener_alto();

    titulo.renderizar(PADDING_X, PADDING_Y, tooltip);
    cuerpo.renderizar(PADDING_X, PADDING_Y + dy, tooltip);

    dy += cuerpo.obtener_alto();
    dy += SEPARACION;
    
    renderizar_metadata(admin_texturas, tooltip, dy, encuadre);
    
    costo.renderizar(PADDING_X, 
        ALTO_TOOLTIP - costo.obtener_alto() - PADDING_Y, tooltip);
    tiempo.renderizar(ANCHO_TOOLTIP - tiempo.obtener_ancho() - PADDING_X, 
        ALTO_TOOLTIP - tiempo.obtener_alto() - PADDING_Y, tooltip);
    
    return tooltip;
}

Textura& Tooltip::obtener_textura(Ventana& ventana) {
    AdministradorTexturas& admin_texturas =
        ventana.obtener_administrador_texturas();
    
    if (admin_texturas.contiene_textura(id_textura))
        return admin_texturas.obtener_textura(id_textura);
    
    return crear_tooltip(admin_texturas);
}

void Tooltip::renderizar(Ventana& ventana, int x, int y) {
    Textura& tooltip = obtener_textura(ventana);
    
    y -= tooltip.obtener_alto();
    if (x + tooltip.obtener_ancho() > ventana.ancho())
        x = ventana.ancho() - tooltip.obtener_ancho();
    
    tooltip.renderizar(x, y);
}

} // namespace cliente