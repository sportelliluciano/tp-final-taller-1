#ifndef _HUD_H_
#define _HUD_H_

#include <list>

#include "cliente/modelo/hud/area_juego.h"
#include "cliente/modelo/hud/boton.h"
#include "cliente/modelo/hud/botonera.h"
#include "cliente/modelo/hud/dinero.h"
#include "cliente/modelo/hud/fps.h"
#include "cliente/modelo/hud/tostador.h"
#include "cliente/modelo/juego.h"
#include "cliente/servidor.h"
#include "cliente/video/ventana.h"
#include "cliente/video/widgets/widget_raiz.h"
#include "cliente/video/widgets/caja_horizontal.h"
#include "cliente/video/widgets/caja_vertical.h"

#include "cliente/modelo/hud/boton_entrenamiento.h"
#include "cliente/modelo/hud/boton_construccion.h"

namespace cliente {

/**
 * \brief Interfaz de usuario del juego.
 */
class HUD : public WidgetRaiz {
public:
    HUD(Ventana& ventana, Juego& juego_, Servidor& servidor_);

    /**
     * \brief Este evento se ejecuta cuando el usuario decide cerrar la ventana.
     */
    virtual bool cerrar_ventana();

private:
    Juego& juego;
    Servidor& servidor;
    AreaJuego area_juego;
    Botonera botonera_construccion, botonera_entrenamiento;
    CajaHorizontal base, barra_superior, barra_botoneras;
    CajaVertical area_general, panel_lateral;

    Boton modo_construccion, modo_entrenamiento, modo_vender;
    
    Tostador tostador;
    
    Boton mutear_sonido, mutear_musica;
    Dinero dinero;
    FPS fps;

    void toggle_sonido();
    void toggle_musica();
    void click_modo_construir();
    void click_modo_entrenar();
    void click_modo_vender();

    bool sonido_activo = true;
    bool musica_activa = false;

    bool shift_presionado = false;

    std::list<BotonConstruccion> botones_construccion;
    std::list<BotonEntrenamiento> botones_entrenamiento;
};

} // namespace cliente

#endif // _HUD_H_
