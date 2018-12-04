#include "cliente/modelo/hud/boton_temporizado.h"

#include <sstream>
#include <string>

#include "cliente/modelo/sprite.h"
#include "cliente/video/color.h"
#include "cliente/video/ventana.h"

#define ANCHO_BOTON 60
#define ALTO_BOTON 50

#define PADDING_X 5
#define PADDING_Y_TIEMPO 20
#define PADDING_Y_COLA 0

#define VERDE Color(0, 127, 0, 128)
#define GRIS  Color(54, 54, 54, 128)

#define PREFIJO_FILTRO "filtro-"

namespace cliente {

BotonTemporizado::BotonTemporizado(int sprite) { 
    sprite_id = sprite;
}

int BotonTemporizado::obtener_alto() const {
    return ALTO_BOTON;
}

int BotonTemporizado::obtener_ancho() const {
    return ANCHO_BOTON;
}

void BotonTemporizado::set_cola(int nueva_cola) {
    cola = nueva_cola;
}

void BotonTemporizado::set_tiempo(int nuevo_tiempo) {
    segundos_restantes = nuevo_tiempo;
}

void BotonTemporizado::reiniciar() {
    cola = segundos_restantes = 0;
    con_filtro_gris = false;
    con_filtro_verde = false;
}

void BotonTemporizado::set_filtro(bool gris, bool verde) {
    con_filtro_gris = gris;
    con_filtro_verde = verde;
}

bool BotonTemporizado::mouse_entra(const Posicion&) {
    mostrar_tooltip = true;
    return false;
}

bool BotonTemporizado::mouse_sale(const Posicion&) {
    mostrar_tooltip = false;
    return false;
}

Textura& BotonTemporizado::obtener_filtro(Ventana& ventana, const Color& color) 
{
    AdministradorTexturas& admin_texturas = 
        ventana.obtener_administrador_texturas();
    
    std::stringstream s_textura;
    s_textura << PREFIJO_FILTRO << color.str();

    const std::string& id_textura = s_textura.str();
    
    if (!admin_texturas.contiene_textura(id_textura)) {
        admin_texturas
            .crear_textura(id_textura, ANCHO_BOTON, ALTO_BOTON)
            .limpiar(color);
    }

    return admin_texturas.obtener_textura(id_textura);
}

void BotonTemporizado::renderizar_filtros(Ventana& ventana, 
    const Posicion& punto) 
{
    if (con_filtro_gris) {
        obtener_filtro(ventana, GRIS).renderizar(punto.x, punto.y);
    } else if (con_filtro_verde) {
        obtener_filtro(ventana, VERDE).renderizar(punto.x, punto.y);
    }
}

void BotonTemporizado::renderizar_cola(Ventana& ventana, const Posicion& punto) 
{
    std::stringstream s;
    s << cola;
    ventana
        .obtener_administrador_texturas()
        .crear_texto(s.str())
        .renderizar(punto.x + PADDING_X, punto.y - PADDING_Y_COLA);
}

void BotonTemporizado::renderizar_tiempo_restante(Ventana& ventana, 
    const Posicion& punto) 
{
    std::stringstream s;
    s << segundos_restantes;
    ventana
        .obtener_administrador_texturas()
        .crear_texto(s.str())
        .renderizar(punto.x + PADDING_X, 
            punto.y + ALTO_BOTON - PADDING_Y_TIEMPO);
}

void BotonTemporizado::renderizar(Ventana& ventana, const Posicion& punto) {
    Sprite(sprite_id).renderizar(ventana, punto.x, punto.y);
    
    renderizar_filtros(ventana, punto);
    
    if (cola > 0)
        renderizar_cola(ventana, punto);
    
    if (segundos_restantes > 0)
        renderizar_tiempo_restante(ventana, punto);

    if (mostrar_tooltip) {
        ventana.cambiar_plano(true);
        tooltip.renderizar(ventana, punto.x, punto.y);
        ventana.cambiar_plano(false);
    }
}

} // namespace cliente
