#include "cliente/modelo/hud/barra_superior.h"

#include "cliente/config.h"
#include "cliente/sonido/sonido.h"

#define ANCHO_BOTON 32

namespace cliente {

BarraSuperior::BarraSuperior(int x, int y, int ancho_, int alto_, Juego& juego_)
: CajaHorizontal(x, y, ancho_, alto_),
  juego(juego_), dinero(juego), energia(juego)
{ 
    crear_boton_salir();
    crear_boton_sonido();
    crear_boton_musica();

    empaquetar_al_frente(salir);
    empaquetar_al_frente(mutear_sonido);
    empaquetar_al_frente(mutear_musica);
    
    empaquetar_al_fondo(energia);
    empaquetar_al_fondo(dinero);
    empaquetar_al_fondo(fps);
}

void BarraSuperior::dimensionar_boton(Boton& boton) {
    boton.set_tamanio(obtener_alto(), ANCHO_BOTON);
    boton.set_padding(5, 4);
}

void BarraSuperior::crear_boton_salir() {
    dimensionar_boton(salir);
    salir.set_imagen(RUTA_IMAGENES "/salir.png");
    salir.registrar_click([this] () { juego.detener(); });
}

void BarraSuperior::crear_boton_sonido() {
    dimensionar_boton(mutear_sonido);
    
    Sonido& sonido = Sonido::obtener_instancia();
    if (sonido.sonido_activo())
        mutear_sonido.set_imagen(RUTA_IMAGENES "/sin-sonido.png");
    else
        mutear_sonido.set_imagen(RUTA_IMAGENES "/con-sonido.png");
    
    mutear_sonido.registrar_click([this](){ btn_sonido_click(); });
}

void BarraSuperior::crear_boton_musica() {
    dimensionar_boton(mutear_musica);
    
    Sonido& sonido = Sonido::obtener_instancia();
    if (!sonido.musica_activa())
        mutear_musica.set_imagen(RUTA_IMAGENES "/musica-habilitada.png");
    else
        mutear_musica.set_imagen(RUTA_IMAGENES "/musica-deshabilitada.png");
    
    mutear_musica.registrar_click([this](){ btn_musica_click(); });
}

void BarraSuperior::btn_sonido_click() {
    Sonido& sonido = Sonido::obtener_instancia();
    if (!sonido.sonido_activo()) {
        mutear_sonido.set_imagen(RUTA_IMAGENES "/sin-sonido.png");
        sonido.habilitar_sonidos(true);
    } else {
        mutear_sonido.set_imagen(RUTA_IMAGENES "/con-sonido.png");
        sonido.habilitar_sonidos(false);
    }
}

void BarraSuperior::btn_musica_click() {
    Sonido& sonido = Sonido::obtener_instancia();
    if (!sonido.musica_activa()) {
        mutear_musica.set_imagen(RUTA_IMAGENES "/musica-deshabilitada.png");
        sonido.iniciar_musica_fondo();
    } else {
        mutear_musica.set_imagen(RUTA_IMAGENES "/musica-habilitada.png");
        sonido.detener_musica_fondo();
    }
}

} // namespace cliente
