#ifndef _EVENTO_DISPARO_H_
#define _EVENTO_DISPARO_H_

#include "libs/json.hpp"

#include "cliente/eventos/evento.h"

namespace cliente {

/**
 * \brief Esta clase encapsula un evento referido a un proyectil que está
 *        en vuelo.
 */
class EventoDisparo : public Evento {
public:
    /**
     * \brief Constructor por serialización.
     */
    EventoDisparo(const nlohmann::json& serializado);

    /**
     * \brief Actualiza el modelo del juego de un cliente.
     */
    void actualizar(Juego& juego);

private:
    nlohmann::json data;
};

} // namespace cliente

#endif // _EVENTO_DISPARO_H_
