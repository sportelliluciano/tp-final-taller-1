#ifndef _EVENTO_MOVER_TROPA_H_
#define _EVENTO_MOVER_TROPA_H_

#include <vector>

#include "libs/json.hpp"

#include "conexion/evento.h"

namespace conexion {

class EventoMoverTropa : public Evento {
public:
    /**
     * \brief Constructor por serialización.
     */
    EventoMoverTropa(const nlohmann::json& serializado);

    /**
     * \brief Constructor.
     */
    EventoMoverTropa(int id_tropa, const std::vector<int>& camino);

    /**
     * \brief Actualiza el modelo del juego de un cliente.
     */
    void actualizar(cliente::Juego& juego);

    /**
     * \brief Devuelve una serialización del objeto en formato JSON.
     */
    nlohmann::json serializar() const;

private:
    int tropa;
    std::vector<int> pasos;
};

} // namespace conexion

#endif // _EVENTO_MOVER_TROPA_H_
