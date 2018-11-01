#ifndef _EVENTO_ACTUALIZAR_DINERO_H_
#define _EVENTO_ACTUALIZAR_DINERO_H_

#include "libs/json.hpp"

#include "conexion/evento.h"

namespace conexion {

class EventoActualizarDinero : public Evento {
public:
    /**
     * \brief Constructor por serialización.
     */
    EventoActualizarDinero(const nlohmann::json& serializado);

    /**
     * \brief Constructor.
     */
    EventoActualizarDinero(int nuevo_dinero);

    /**
     * \brief Actualiza el modelo del juego de un cliente.
     */
    void actualizar(cliente::Juego& juego);

    /**
     * \brief Devuelve una serialización del objeto en formato JSON.
     */
    nlohmann::json serializar() const;

private:
    int nuevo_dinero;
};

} // namespace conexion

#endif // _EVENTO_ACTUALIZAR_DINERO_H_
