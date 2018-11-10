#ifndef _HUD_H_
#define _HUD_H_

#include "cliente/modelo/hud/area_juego.h"
#include "cliente/modelo/hud/boton.h"
#include "cliente/modelo/hud/botonera.h"
#include "cliente/modelo/hud/dinero.h"
#include "cliente/modelo/hud/fps.h"
#include "cliente/modelo/juego.h"
#include "cliente/servidor.h"
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
     * \brief Se ejecuta cuando el usuario presiona una tecla.
     */
    virtual bool teclado_presionado(tecla_t tecla);

    /**
     * \brief Se ejecuta cuando el usuario suelta una tecla.
     */
    virtual bool teclado_suelto(tecla_t tecla);

    /**
     * \brief Este evento se ejecuta cuando el usuario decide cerrar la ventana.
     */
    virtual bool cerrar_ventana();

private:
    Juego& juego;
    Servidor& servidor;
    AreaJuego area_juego;
    Botonera botonera_lateral;
    CajaVertical base, panel_lateral;
    CajaHorizontal barra_superior, area_general;
    
    Boton mutear_sonido, mutear_musica;
    Dinero dinero;
    FPS fps;

    void toggle_sonido();
    void toggle_musica();
    void construir_edificio(const std::string& clase);

    bool sonido_activo = true;
    bool musica_activa = false;

    bool shift_presionado = false;
};

} // namespace cliente

#endif // _HUD_H_
