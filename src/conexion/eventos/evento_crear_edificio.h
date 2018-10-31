#ifndef _EVENTO_CREAR_EDIFICIO_H_
#define _EVENTO_CREAR_EDIFICIO_H_

#include "libs/json.hpp"

#include "conexion/evento.h"

namespace conexion {

class EventoCrearEdificio : public Evento {
public:
    /**
     * \brief Constructor por serialización.
     */
    EventoCrearEdificio(const nlohmann::json& serializado);

    /**
     * \brief Constructor.
     */
    EventoCrearEdificio(const std::string& clase_, int x_, int y_);

    /**
     * \brief Actualiza el modelo del juego de un cliente.
     */
    void actualizar(cliente::Juego& juego);

    /**
     * \brief Devuelve una serialización del objeto en formato JSON.
     */
    nlohmann::json serializar() const;

private:
    std::string clase;
    int x, y;
};

} // namespace conexion

#endif // _EVENTO_CREAR_EDIFICIO_H_
