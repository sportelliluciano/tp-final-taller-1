#ifndef _EVENTO_TERMINAR_H_
#define _EVENTO_TERMINAR_H_

#include "cliente/modelo/eventos/evento.h"

namespace cliente {

/**
 * \brief Evento especial que detiene el juego.
 */
class EventoTerminar : public Evento {
public:
    EventoTerminar(const nlohmann::json& serializado);

    /**
     * \brief Actualiza el modelo del juego de un cliente.
     */
    void actualizar(Juego& juego);

private:
    nlohmann::json data;
};

} // namespace cliente

#endif // _EVENTO_TERMINAR_H_
