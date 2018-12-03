#ifndef _EVENTO_INFRAESTRUCTURA_H_
#define _EVENTO_INFRAESTRUCTURA_H_

#include "libs/json.hpp"

#include "cliente/modelo/eventos/evento.h"

namespace cliente {

/**
 * \brief Esta clase encapsula un evento referido a la infraestructura.
 */
class EventoInfraestructura : public Evento {
public:
    /**
     * \brief Constructor por serialización.
     */
    EventoInfraestructura(const nlohmann::json& serializado);

    /**
     * \brief Actualiza el modelo del juego de un cliente.
     */
    void actualizar(Juego& juego);

private:
    nlohmann::json data;
};

} // namespace cliente

#endif // _EVENTO_INFRAESTRUCTURA_H_
