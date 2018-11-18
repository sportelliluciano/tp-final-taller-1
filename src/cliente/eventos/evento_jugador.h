#ifndef _EVENTO_JUGADOR_H_
#define _EVENTO_JUGADOR_H_

#include "libs/json.hpp"

#include "cliente/eventos/evento.h"

namespace cliente {

/**
 * \brief Esta clase encapsula un evento referido al estado del jugador.
 */
class EventoJugador : public Evento {
public:
    /**
     * \brief Constructor por serialización.
     */
    EventoJugador(const nlohmann::json& serializado);

    /**
     * \brief Actualiza el modelo del juego de un cliente.
     */
    void actualizar(Juego& juego);

private:
    nlohmann::json data;
};

} // namespace cliente

#endif // _EVENTO_JUGADOR_H_
