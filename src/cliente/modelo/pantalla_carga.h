#ifndef _PANTALLA_CARGA_H_
#define _PANTALLA_CARGA_H_

#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Muestra una pantalla de carga mientras se espera a los otros 
 *        jugadores.
 */
class PantallaCarga {
public:
    void renderizar(Ventana& ventana);
};

} // namespace cliente

#endif // _PANTALLA_CARGA_H_
