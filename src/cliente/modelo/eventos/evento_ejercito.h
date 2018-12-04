#ifndef _EVENTO_EJERCITO_H_
#define _EVENTO_EJERCITO_H_

#include "libs/json.hpp"

#include "cliente/modelo/eventos/evento.h"

namespace cliente {

/**
 * \brief Esta clase encapsula un evento referido a la infraestructura.
 */
class EventoEjercito : public Evento {
public:
    /**
     * \brief Constructor por serialización.
     */
    EventoEjercito(const nlohmann::json& serializado);

    /**
     * \brief Actualiza el modelo del juego de un cliente.
     */
    void actualizar(Juego& juego);

private:
    nlohmann::json data;
};

} // namespace cliente

#endif // _EVENTO_EJERCITO_H_
