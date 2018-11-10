#ifndef _CONTROLADOR_H_
#define _CONTROLADOR_H_

#include "cliente/modelo/hud.h"
#include "cliente/modelo/juego.h"
#include "cliente/servidor.h"
#include "cliente/video/ventana.h"
#include "cliente/video/widgets/widget_raiz.h"

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

    /**
     * \brief Renderiza el HUD.
     */
    void renderizar();

    /**
     * \brief Procesa las distintas entradas y actualiza el modelo de juego.
     */
    void procesar_entrada();

private:
    Ventana& ventana;
    Servidor& servidor;
    Juego& juego;

    HUD hud;
};

} // namespace cliente

#endif // _CONTROLADOR_H_
