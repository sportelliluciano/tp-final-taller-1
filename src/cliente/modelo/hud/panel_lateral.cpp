#include "cliente/modelo/hud/panel_lateral.h"

#include "cliente/modelo/hud/area_juego.h"
#include "cliente/modelo/hud/boton.h"
#include "cliente/config.h"
#include "cliente/modelo/hud/panel_lateral/botonera.h"
#include "cliente/modelo/hud/panel_lateral/boton_entrenamiento.h"
#include "cliente/modelo/hud/panel_lateral/boton_construccion.h"

#include "cliente/video/widgets/caja_vertical.h"
#include "cliente/video/widgets/caja_horizontal.h"

#define ALTO_BARRA_BOTONERA 42

namespace cliente {

PanelLateral::PanelLateral(int x, int y, int ancho, int alto, 
    AreaJuego& area_juego_, Juego& juego, Servidor& servidor, Tostador& tostador)
: CajaVertical(x, y, ancho, alto),
  area_juego(area_juego_),
  botonera_construccion(ancho, alto / 2),
  botonera_entrenamiento(ancho, alto / 2),
  barra_botoneras(x, y, ancho, ALTO_BARRA_BOTONERA)
{ 
    barra_botoneras.empaquetar_al_frente(modo_construccion);
    barra_botoneras.empaquetar_al_frente(modo_entrenamiento);
    barra_botoneras.empaquetar_al_frente(modo_vender);

    modo_construccion.set_imagen(RUTA_IMAGENES "/icono-construir.png");
    modo_construccion.set_tamanio(ancho / 3, ALTO_BARRA_BOTONERA);
    modo_construccion.centrar_imagen(true);
    modo_construccion.registrar_click([this](){ click_modo_construir(); });
    
    modo_entrenamiento.set_imagen(RUTA_IMAGENES "/icono-entrenar.png");
    modo_entrenamiento.set_tamanio(ancho / 3, ALTO_BARRA_BOTONERA);
    modo_entrenamiento.centrar_imagen(true);
    modo_entrenamiento.registrar_click([this](){ click_modo_entrenar(); });
    
    modo_vender.set_imagen(RUTA_IMAGENES "/icono-vender.png");
    modo_vender.set_tamanio(ancho / 3, ALTO_BARRA_BOTONERA);
    modo_vender.centrar_imagen(true);
    modo_vender.registrar_click([this](){ click_modo_vender(); });

    crear_botones_construir(area_juego, juego, servidor, tostador);
    crear_botones_entrenar(juego, servidor, tostador);

    empaquetar_abajo(barra_botoneras);
    empaquetar_abajo(botonera_construccion);
}

void PanelLateral::crear_botones_construir(AreaJuego& area_juego_, Juego& juego, 
    Servidor& servidor, Tostador& tostador)
{
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
}

void PanelLateral::crear_botones_entrenar(Juego& juego, Servidor& servidor, 
    Tostador& tostador) 
{
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
}

void PanelLateral::click_modo_construir() {
    area_juego.set_modo_vender(false);
    reemplazar_widget(botonera_entrenamiento, 
        botonera_construccion);
}

void PanelLateral::click_modo_entrenar() {
    area_juego.set_modo_vender(false);
    reemplazar_widget(botonera_construccion, 
        botonera_entrenamiento);
}

void PanelLateral::click_modo_vender() {
    area_juego.set_modo_vender(true);
}

} // namespace cliente
