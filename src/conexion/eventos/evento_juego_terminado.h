#ifndef _EVENTO_JUEGO_TERMINADO_H_
#define _EVENTO_JUEGO_TERMINADO_H_

#include "libs/json.hpp"

#include "conexion/evento.h"

namespace conexion {

class EventoJuegoTerminado : public Evento {
public:
    /**
     * \brief Constructor por serialización.
     */
    EventoJuegoTerminado(const nlohmann::json& serializado);

    /**
     * \brief Constructor.
     */
    EventoJuegoTerminado() = default;

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
    std::string clase;
    int x, y;
};

} // namespace conexion

#endif // _EVENTO_JUEGO_TERMINADO_H_
