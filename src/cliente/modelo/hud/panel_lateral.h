#ifndef _PANEL_LATERAL_H_
#define _PANEL_LATERAL_H_

#include <list>

#include "cliente/modelo/hud/area_juego.h"
#include "cliente/modelo/hud/boton.h"
#include "cliente/modelo/hud/panel_lateral/botonera.h"
#include "cliente/modelo/hud/panel_lateral/boton_entrenamiento.h"
#include "cliente/modelo/hud/panel_lateral/boton_construccion.h"

#include "cliente/video/widgets/caja_vertical.h"
#include "cliente/video/widgets/caja_horizontal.h"

namespace cliente { 

/**
 * \brief Panel lateral del juego, muestra el minimapa y los botones de
 *        construcción / entrenamiento.
 */
class PanelLateral : public CajaVertical {
public:
    PanelLateral(int x, int y, int ancho, int alto, AreaJuego& area_juego_, 
        Juego& juego, Servidor& servidor, Tostador& tostador);

private:
    AreaJuego& area_juego;
    Botonera botonera_construccion, botonera_entrenamiento;

    CajaHorizontal barra_botoneras;
    Boton modo_construccion, modo_entrenamiento, modo_vender;

    std::list<BotonConstruccion> botones_construccion;
    std::list<BotonEntrenamiento> botones_entrenamiento;

    /**
     * \brief Controladores de eventos.
     * 
     * Los siguientes métodos controlan el click en cada uno de los botones.
     */
    void click_modo_construir();
    void click_modo_entrenar();
    void click_modo_vender();

    /**
     * \brief Crea la botonera de construcciones.
     */
    void crear_botones_construir(AreaJuego& area_juego_, Juego& juego, 
        Servidor& servidor, Tostador& tostador);
    
    /**
     * \brief Crea la botonera de entrenamientos.
     */
    void crear_botones_entrenar(Juego& juego, 
        Servidor& servidor, Tostador& tostador);
};

} // namespace cliente

#endif // _PANEL_LATERAL_H_
