#ifndef _BARRA_SUPERIOR_H_
#define _BARRA_SUPERIOR_H_

#include "cliente/modelo/juego.h"
#include "cliente/modelo/hud/boton.h"
#include "cliente/modelo/hud/barra_superior/dinero.h"
#include "cliente/modelo/hud/barra_superior/energia.h"
#include "cliente/modelo/hud/barra_superior/fps.h"
#include "cliente/video/widgets/caja_horizontal.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Barra superior de la interfaz de usuario del juego.
 */
class BarraSuperior : public CajaHorizontal {
public:
    /**
     * \brief Crea una barra superior en la posición y con las dimensiones
     *        indicadas.
     */
    BarraSuperior(int x, int y, int ancho_, int alto_, Juego& juego_);

private:
    Juego& juego;

    Boton mutear_sonido, mutear_musica, salir;
    Dinero dinero;
    Energia energia;
    FPS fps;

    /**
     * \brief Asigna al botón el tamaño y padding requerido.
     */
    void dimensionar_boton(Boton& boton);

    /**
     * \brief Los siguientes métodos inicializan los botones con las imágenes
     *        y acciones correspondientes.
     */
    void crear_boton_salir();
    void crear_boton_musica();
    void crear_boton_sonido();

    /**
     * \brief Controladores de eventos.
     * 
     * Los siguientes métodos controlan el click en los botones.
     */
    void btn_sonido_click();
    void btn_musica_click();
};

} // namespace cliente

#endif // _BARRA_SUPERIOR_H_
