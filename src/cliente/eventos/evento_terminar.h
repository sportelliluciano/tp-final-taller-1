#ifndef _EVENTO_TERMINAR_H_
#define _EVENTO_TERMINAR_H_

#include "cliente/eventos/evento.h"

namespace cliente {

/**
 * \brief Evento especial que detiene el juego.
 */
class EventoTerminar : public Evento {
public:
    EventoTerminar() = default;

    /**
     * \brief Actualiza el modelo del juego de un cliente.
     */
    void actualizar(Juego& juego);
};

} // namespace cliente

#endif // _EVENTO_TERMINAR_H_
