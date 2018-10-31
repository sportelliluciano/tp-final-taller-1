#ifndef _EVENTO_DESTRUIR_EDIFICIO_H_
#define _EVENTO_DESTRUIR_EDIFICIO_H_

#include "libs/json.hpp"

#include "conexion/evento.h"

namespace conexion {

class EventoDestruirEdificio : public Evento {
public:
    /**
     * \brief Constructor por serialización.
     */
    EventoDestruirEdificio(const nlohmann::json& serializado);

    /**
     * \brief Constructor.
     */
    EventoDestruirEdificio(int id_);

    /**
     * \brief Actualiza el modelo del juego de un cliente.
     */
    void actualizar(cliente::Juego& juego);

    /**
     * \brief Devuelve una serialización del objeto en formato JSON.
     */
    nlohmann::json serializar() const;

private:
    int id;
};

} // namespace conexion

#endif // _EVENTO_DESTRUIR_EDIFICIO_H_
