#include "cliente/modelo/hud.h"

#include <iostream>

#include "cliente/modelo/hud/area_juego.h"
#include "cliente/modelo/hud/tostador.h"
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
#define ALTO_BARRA_BOTONERA 40

#define MARGEN_TOSTADOR_X 30
#define MARGEN_TOSTADOR_Y (ALTO_BARRA_SUPERIOR + 20)

namespace cliente {

HUD::HUD(Ventana& ventana, Juego& juego_, Servidor& servidor_) 
: WidgetRaiz(ventana.ancho(), ventana.alto()),
  juego(juego_),
  servidor(servidor_),
  area_juego(juego_, servidor, tostador), 
  botonera_construccion(ANCHO_BOTONERA, ventana.alto() / 2),
  botonera_entrenamiento(ANCHO_BOTONERA, ventana.alto() / 2),
  base(0, 0, ventana.ancho(), ventana.alto()), 
  barra_superior(0, 0, ventana.ancho() - ANCHO_BOTONERA, ALTO_BARRA_SUPERIOR),
  barra_botoneras(ventana.ancho() - ANCHO_BOTONERA, 
    ventana.alto() / 2 - ALTO_BARRA_BOTONERA, ANCHO_BOTONERA, 
    ALTO_BARRA_BOTONERA),
  area_general(0, ALTO_BARRA_SUPERIOR, ventana.ancho() - ANCHO_BOTONERA, 
    ventana.alto() - ALTO_BARRA_SUPERIOR),
  panel_lateral(ventana.ancho() - ANCHO_BOTONERA, 0, 
    ANCHO_BOTONERA, ventana.alto()),
  tostador(MARGEN_TOSTADOR_X, MARGEN_TOSTADOR_Y)
{
    setear_hijo(&base);
    base.empaquetar_al_frente(area_general);
    base.empaquetar_al_frente(panel_lateral);
    base.empaquetar_al_frente(tostador);
    area_general.empaquetar_arriba(barra_superior);
    area_general.empaquetar_arriba(area_juego);

    panel_lateral.empaquetar_abajo(barra_botoneras);
    panel_lateral.empaquetar_abajo(botonera_construccion);
    
    
    barra_botoneras.empaquetar_al_frente(modo_construccion);
    barra_botoneras.empaquetar_al_frente(modo_entrenamiento);
    barra_botoneras.empaquetar_al_frente(modo_vender);

    modo_construccion.set_imagen("./assets/nuevos/icono-construir.png");
    modo_construccion.set_tamanio(ANCHO_BOTONERA / 3, ALTO_BARRA_BOTONERA);
    modo_construccion.set_autopadding(true);
    modo_construccion.registrar_click([this](){ click_modo_construir(); });
    
    modo_entrenamiento.set_imagen("./assets/nuevos/icono-entrenar.png");
    modo_entrenamiento.set_tamanio(ANCHO_BOTONERA / 3, ALTO_BARRA_BOTONERA);
    modo_entrenamiento.set_autopadding(true);
    modo_entrenamiento.registrar_click([this](){ click_modo_entrenar(); });
    
    modo_vender.set_imagen("./assets/nuevos/icono-vender.png");
    modo_vender.set_tamanio(ANCHO_BOTONERA / 3, ALTO_BARRA_BOTONERA);
    modo_vender.set_autopadding(true);
    modo_vender.registrar_click([this](){ click_modo_vender(); });    
    

    mutear_sonido.set_tamanio(42, 32);
    mutear_sonido.set_imagen("./assets/nuevos/sin-sonido.png");
    mutear_sonido.set_padding(5, 4);
    mutear_sonido.registrar_click([this](){ toggle_sonido(); });
    
    mutear_musica.set_tamanio(42, 32);
    mutear_musica.set_imagen("./assets/nuevos/musica-habilitada.png");
    mutear_musica.set_padding(5, 4);
    mutear_musica.registrar_click([this](){ toggle_musica(); });

    for (const Edificio* edificio : juego.obtener_edificios()) {
        botones_construccion.emplace_back(
            BotonConstruccion(
                juego.obtener_infraestructura(), 
                edificio->obtener_clase(), 
                servidor,
                tostador
            )
        );
        botonera_construccion.agregar_widget(botones_construccion.back());
        botones_construccion.back().en_ubicar_nuevo_edificio(
            [this, edificio] () { area_juego.ubicar_edificio(edificio); });
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
    switch(tecla) {
        case TECLA_SHIFT:
            shift_presionado = true;
            area_juego.set_modo_vender(true);
            break;
        case TECLA_IZQUIERDA:
        case TECLA_DERECHA:
        case TECLA_ABAJO:
        case TECLA_ARRIBA:
        case TECLA_CTRL:
            return area_juego.teclado_presionado(tecla);
        default:
            break;
    }
    
    return false;
}

bool HUD::teclado_suelto(tecla_t tecla) {
    switch(tecla) {
        case TECLA_SHIFT:
            shift_presionado = true;
            area_juego.set_modo_vender(false);
            break;
        case TECLA_IZQUIERDA:
        case TECLA_DERECHA:
        case TECLA_ABAJO:
        case TECLA_ARRIBA:
        case TECLA_CTRL:
            return area_juego.teclado_suelto(tecla);
        default:
            break;
    }
    
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

bool HUD::cerrar_ventana() {
    juego.detener();
    servidor.detener();
    return false;
}

void HUD::click_modo_construir() {
    area_juego.set_modo_vender(false);
    panel_lateral.reemplazar_widget(botonera_entrenamiento, 
        botonera_construccion);
}

void HUD::click_modo_entrenar() {
    area_juego.set_modo_vender(false);
    panel_lateral.reemplazar_widget(botonera_construccion, 
        botonera_entrenamiento);
}

void HUD::click_modo_vender() {
    area_juego.set_modo_vender(true);
}


} // namespace cliente
