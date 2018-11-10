#include "cliente/modelo/hud.h"

#include <iostream>

#include "cliente/modelo/hud/area_juego.h"
#include "cliente/modelo/juego.h"
#include "cliente/servidor.h"
#include "cliente/sonido/sonido.h"
#include "cliente/video/ventana.h"
#include "cliente/video/widgets/widget.h"
#include "cliente/video/widgets/widget_raiz.h"
#include "cliente/video/widgets/caja_vertical.h"
#include "cliente/video/widgets/caja_horizontal.h"

#define ANCHO_BOTONERA 250
#define ALTO_BARRA_SUPERIOR 42

namespace cliente {

HUD::HUD(Ventana& ventana, Juego& juego_, Servidor& servidor_) 
: WidgetRaiz(ventana.ancho(), ventana.alto()),
  juego(juego_),
  servidor(servidor_),
  area_juego(juego_), 
  botonera_lateral(ANCHO_BOTONERA, ventana.alto() / 2),
  base(0, 0, ventana.ancho(), ventana.alto()), 
  panel_lateral(ventana.ancho() - ANCHO_BOTONERA, ALTO_BARRA_SUPERIOR, 
    ANCHO_BOTONERA, ventana.alto() - ALTO_BARRA_SUPERIOR),
  barra_superior(0, 0, ventana.ancho(), ALTO_BARRA_SUPERIOR),
  area_general(0, ALTO_BARRA_SUPERIOR, ventana.ancho() - ANCHO_BOTONERA, 
    ventana.alto() - ALTO_BARRA_SUPERIOR)
{
    setear_hijo(&base);
    base.empaquetar_arriba(barra_superior);
    base.empaquetar_arriba(area_general);
    area_general.empaquetar_al_frente(area_juego);
    area_general.empaquetar_al_frente(panel_lateral);
    panel_lateral.empaquetar_abajo(botonera_lateral);

    mutear_sonido.set_tamanio(42, 32);
    mutear_sonido.set_imagen("./assets/nuevos/sin-sonido.png");
    mutear_sonido.set_padding(5, 4);
    mutear_sonido.registrar_click([this](){ toggle_sonido(); });
    
    mutear_musica.set_tamanio(42, 32);
    mutear_musica.set_imagen("./assets/nuevos/musica-habilitada.png");
    mutear_musica.set_padding(5, 4);
    mutear_musica.registrar_click([this](){ toggle_musica(); });

    for (const Edificio* edificio : juego.obtener_edificios()) {
        Boton& btn = *botonera_lateral.crear_boton();
        btn.set_sprite(edificio->obtener_sprite_boton());
        btn.registrar_click([this, edificio] () { 
            construir_edificio(edificio->obtener_clase());
        });
    }

    barra_superior.empaquetar_al_frente(mutear_sonido);
    barra_superior.empaquetar_al_frente(mutear_musica);
    barra_superior.empaquetar_al_fondo(dinero);
    barra_superior.empaquetar_al_fondo(fps);

    area_juego.set_tamanio(
        ventana.ancho() - ANCHO_BOTONERA,
        ventana.alto() - barra_superior.obtener_alto());    
}

bool HUD::teclado_presionado(tecla_t tecla) {
    if (tecla == TECLA_SHIFT)
        shift_presionado = true;
    return false;
}

bool HUD::teclado_suelto(tecla_t tecla) {
    if (tecla == TECLA_SHIFT)
        shift_presionado = false;
    return false;
}

void HUD::toggle_sonido() {
    if (!sonido_activo) {
        mutear_sonido.set_imagen("./assets/nuevos/sin-sonido.png");
        Sonido::obtener_instancia().set_volumen_sonidos(100);
    } else {
        mutear_sonido.set_imagen("./assets/nuevos/con-sonido.png");
        Sonido::obtener_instancia().set_volumen_sonidos(0);
    }

    sonido_activo = !sonido_activo;
}

void HUD::toggle_musica() {
    if (!musica_activa) {
        mutear_musica.set_imagen("./assets/nuevos/musica-deshabilitada.png");
        Sonido::obtener_instancia().iniciar_musica_fondo();
    } else {
        mutear_musica.set_imagen("./assets/nuevos/musica-habilitada.png");
        Sonido::obtener_instancia().detener_musica_fondo();
    }

    musica_activa = !musica_activa;
}

void HUD::construir_edificio(const std::string& clase) {
    std::cout << "Construyendo: " << clase << std::endl;
    Sonido::obtener_instancia().reproducir_sonido(SONIDO_BLEEP);
}

bool HUD::cerrar_ventana() {
    juego.detener();
    servidor.detener();
    return false;
}

} // namespace cliente
