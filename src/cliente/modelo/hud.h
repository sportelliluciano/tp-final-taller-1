#ifndef _HUD_H_
#define _HUD_H_

#include <list>

#include "cliente/modelo/juego.h"
#include "cliente/modelo/hud/area_juego.h"
#include "cliente/modelo/hud/barra_superior.h"
#include "cliente/modelo/hud/panel_lateral.h"

#include "cliente/red/servidor.h"

#include "cliente/video/ventana.h"
#include "cliente/video/widgets/widget_raiz.h"
#include "cliente/video/widgets/caja_horizontal.h"
#include "cliente/video/widgets/caja_vertical.h"

namespace cliente {

/**
 * \brief Interfaz de usuario del juego.
 */
class HUD : public WidgetRaiz {
public:
    HUD(Ventana& ventana, Juego& juego_, Servidor& servidor_);

    /**
     * \brief Es llamado al iniciar el juego.
     * 
     * Centra la cámara en el centro de construcciones.
     */
    void inicializar_juego();

    /**
     * \brief Este evento se ejecuta cuando el usuario decide cerrar la ventana.
     */
    virtual bool cerrar_ventana();

private:
    Juego& juego;
    Tostador tostador;
    
    CajaHorizontal base;
    
    CajaVertical panel_izquierdo;
    BarraSuperior barra_superior;
    AreaJuego area_juego;

    PanelLateral panel_derecho;
};

} // namespace cliente

#endif // _HUD_H_
