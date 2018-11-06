#ifndef _CONTROLADOR_H_
#define _CONTROLADOR_H_

#include "cliente/modelo/hud.h"
#include "cliente/modelo/juego.h"
#include "cliente/servidor.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Controlador del juego.
 * 
 * Esta clase se encarga de actualizar el modelo desde los datos del servidor y
 * desde la entrada del usuario.
 */ 
class Controlador {
public:
    Controlador(Ventana& ventana_, Servidor& servidor_, Juego& juego_);

    void actualizar_modelo();

    void renderizar();

    void procesar_entrada();

private:
    Ventana& ventana;
    Servidor& servidor;
    Juego& juego;
    
    HUD hud;

    void mouse_click_derecho(int x, int y, bool up);
    void mouse_click_izquierdo(int x, int y, bool up);
    void mouse_rueda(int y);
    void recibir_teclado(tecla_t tecla, bool up);

    /**
     * \brief Donde se encuentra el mouse en este cuadro.
     */
    int mouse_x, mouse_y;

    bool draggeando = false;

    int drag_start_x, drag_start_y, drag_end_x, drag_end_y;
};

} // namespace cliente

#endif // _CONTROLADOR_H_
