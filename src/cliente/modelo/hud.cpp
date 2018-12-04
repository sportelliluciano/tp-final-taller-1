#include "cliente/modelo/hud.h"

#include "cliente/config.h"
#include "cliente/modelo/hud/area_juego.h"
#include "cliente/modelo/hud/energia.h"
#include "cliente/modelo/hud/tostador.h"
#include "cliente/modelo/juego.h"
#include "cliente/red/servidor.h"
#include "cliente/sonido/sonido.h"
#include "cliente/video/ventana.h"
#include "cliente/video/widgets/widget.h"
#include "cliente/video/widgets/widget_raiz.h"
#include "cliente/video/widgets/caja_vertical.h"
#include "cliente/video/widgets/caja_horizontal.h"
#include "cliente/modelo/hud/boton_entrenamiento.h"
#include "cliente/modelo/hud/boton_construccion.h"

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
  tostador(MARGEN_TOSTADOR_X, MARGEN_TOSTADOR_Y),
  dinero(juego),
  energia(juego)
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

    modo_construccion.set_imagen(RUTA_IMAGENES "/icono-construir.png");
    modo_construccion.set_tamanio(ANCHO_BOTONERA / 3, ALTO_BARRA_BOTONERA);
    modo_construccion.centrar_imagen(true);
    modo_construccion.registrar_click([this](){ click_modo_construir(); });
    
    modo_entrenamiento.set_imagen(RUTA_IMAGENES "/icono-entrenar.png");
    modo_entrenamiento.set_tamanio(ANCHO_BOTONERA / 3, ALTO_BARRA_BOTONERA);
    modo_entrenamiento.centrar_imagen(true);
    modo_entrenamiento.registrar_click([this](){ click_modo_entrenar(); });
    
    modo_vender.set_imagen(RUTA_IMAGENES "/icono-vender.png");
    modo_vender.set_tamanio(ANCHO_BOTONERA / 3, ALTO_BARRA_BOTONERA);
    modo_vender.centrar_imagen(true);
    modo_vender.registrar_click([this](){ click_modo_vender(); });    
    
    salir.set_tamanio(42, 32);
    salir.set_imagen(RUTA_IMAGENES "/salir.png");
    salir.set_padding(5, 4);
    salir.registrar_click([this] () { juego.detener(); });

    Sonido& sonido = Sonido::obtener_instancia();
    mutear_sonido.set_tamanio(42, 32);
    if (sonido.sonido_activo())
        mutear_sonido.set_imagen(RUTA_IMAGENES "/sin-sonido.png");
    else
        mutear_sonido.set_imagen(RUTA_IMAGENES "/con-sonido.png");
    mutear_sonido.set_padding(5, 4);
    mutear_sonido.registrar_click([this](){ toggle_sonido(); });
    
    mutear_musica.set_tamanio(42, 32);
    if (!sonido.musica_activa())
        mutear_musica.set_imagen(RUTA_IMAGENES "/musica-habilitada.png");
    else
        mutear_musica.set_imagen(RUTA_IMAGENES "/musica-deshabilitada.png");
    mutear_musica.set_padding(5, 4);
    mutear_musica.registrar_click([this](){ toggle_musica(); });

    for (const Edificio* edificio : 
        juego.obtener_infraestructura().obtener_edificios_base()) 
    {
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

    for (const Tropa* tropa : juego.obtener_ejercito().obtener_tropas_base()) {
        if (!tropa->casa_puede_entrenar(juego.obtener_casa_jugador()))
            continue;
        botones_entrenamiento.emplace_back(
            BotonEntrenamiento(
                juego.obtener_ejercito(), 
                tropa->obtener_clase(), 
                servidor,
                tostador
            )
        );
        botonera_entrenamiento.agregar_widget(botones_entrenamiento.back());
    }

    barra_superior.empaquetar_al_frente(salir);
    barra_superior.empaquetar_al_frente(mutear_sonido);
    barra_superior.empaquetar_al_frente(mutear_musica);
    barra_superior.empaquetar_al_fondo(energia);
    barra_superior.empaquetar_al_fondo(dinero);
    barra_superior.empaquetar_al_fondo(fps);

    area_juego.set_tamanio(
        ventana.ancho() - ANCHO_BOTONERA,
        ventana.alto() - barra_superior.obtener_alto());
}

void HUD::toggle_sonido() {
    Sonido& sonido = Sonido::obtener_instancia();
    if (!sonido.sonido_activo()) {
        mutear_sonido.set_imagen(RUTA_IMAGENES "/sin-sonido.png");
        sonido.habilitar_sonidos(true);
    } else {
        mutear_sonido.set_imagen(RUTA_IMAGENES "/con-sonido.png");
        sonido.habilitar_sonidos(false);
    }
}

void HUD::toggle_musica() {
    Sonido& sonido = Sonido::obtener_instancia();
    if (!sonido.musica_activa()) {
        mutear_musica.set_imagen(RUTA_IMAGENES "/musica-deshabilitada.png");
        sonido.iniciar_musica_fondo();
    } else {
        mutear_musica.set_imagen(RUTA_IMAGENES "/musica-habilitada.png");
        sonido.detener_musica_fondo();
    }
}

bool HUD::cerrar_ventana() {
    juego.detener();
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
