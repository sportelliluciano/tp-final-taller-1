#ifndef _CONTROLADOR_H_
#define _CONTROLADOR_H_

#include "cliente/modelo/hud.h"
#include "cliente/modelo/juego.h"
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
    Controlador(Ventana& ventana_, Juego& juego_);
    void actualizar_modelo();
private:
    Juego& juego;
    Ventana& ventana;
    HUD hud;
};

} // namespace cliente

#endif // _CONTROLADOR_H_
