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

private:
    Ventana& ventana;
    Servidor& servidor;
    Juego& juego;
    
    HUD hud;
};

} // namespace cliente

#endif // _CONTROLADOR_H_
