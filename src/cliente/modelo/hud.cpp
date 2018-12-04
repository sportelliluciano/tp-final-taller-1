#include "cliente/modelo/hud.h"

#include "cliente/modelo/juego.h"
#include "cliente/modelo/hud/area_juego.h"
#include "cliente/modelo/hud/barra_superior.h"
#include "cliente/modelo/hud/panel_lateral.h"

#include "cliente/red/servidor.h"

#include "cliente/video/ventana.h"
#include "cliente/video/widgets/widget_raiz.h"
#include "cliente/video/widgets/caja_vertical.h"
#include "cliente/video/widgets/caja_horizontal.h"

#define ANCHO_PANEL_LATERAL 250
#define ALTO_BARRA_SUPERIOR 42

#define MARGEN_TOSTADOR_X 30
#define MARGEN_TOSTADOR_Y 20

namespace cliente {

HUD::HUD(Ventana& ventana, Juego& juego_, Servidor& servidor_) 
: WidgetRaiz(ventana.ancho(), ventana.alto()),
  juego(juego_),
  tostador(MARGEN_TOSTADOR_X, MARGEN_TOSTADOR_Y + ALTO_BARRA_SUPERIOR),
  base(0, 0, ventana.ancho(), ventana.alto()),
  panel_izquierdo(0, 0, ventana.ancho() - ANCHO_PANEL_LATERAL, ventana.alto()),
  barra_superior(0, 0, ventana.ancho() - ANCHO_PANEL_LATERAL, 
    ALTO_BARRA_SUPERIOR, juego),
  area_juego(juego_, servidor_, tostador), 
  panel_derecho(ventana.ancho() - ANCHO_PANEL_LATERAL, 0, ANCHO_PANEL_LATERAL, 
    ventana.alto(), area_juego, juego, servidor_, tostador)
{
    setear_hijo(&base);

    base.empaquetar_al_frente(panel_izquierdo);
    base.empaquetar_al_frente(panel_derecho);
    base.empaquetar_al_frente(tostador);
    
    panel_izquierdo.empaquetar_arriba(barra_superior);
    panel_izquierdo.empaquetar_arriba(area_juego);

    area_juego.set_tamanio(
        ventana.ancho() - ANCHO_PANEL_LATERAL,
        ventana.alto() - barra_superior.obtener_alto());
}

void HUD::inicializar_juego() {
    area_juego.centrar_camara();
}

bool HUD::cerrar_ventana() {
    juego.detener();
    return false;
}

} // namespace cliente
