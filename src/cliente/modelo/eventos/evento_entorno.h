#ifndef _EVENTO_ENTORNO_H_
#define _EVENTO_ENTORNO_H_

#include "libs/json.hpp"

#include "cliente/modelo/eventos/evento.h"

namespace cliente {

/**
 * \brief Esta clase encapsula un evento referido al estado general del juego.
 */
class EventoEntorno : public Evento {
public:
    /**
     * \brief Constructor por serialización.
     */
    EventoEntorno(const nlohmann::json& serializado);

    /**
     * \brief Actualiza el modelo del juego de un cliente.
     */
    void actualizar(Juego& juego);

private:
    nlohmann::json data;
};

} // namespace cliente

#endif // _EVENTO_ENTORNO_H_
