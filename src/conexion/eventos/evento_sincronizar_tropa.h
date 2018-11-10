#ifndef _EVENTO_SINCRONIZAR_TROPA_H_
#define _EVENTO_SINCRONIZAR_TROPA_H_

#include "libs/json.hpp"

#include "conexion/evento.h"

namespace conexion {

class EventoSincronizarTropa : public Evento {
public:
    /**
     * \brief Constructor por serialización.
     */
    EventoSincronizarTropa(const nlohmann::json& serializado);

    /**
     * \brief Constructor.
     */
    EventoSincronizarTropa(int id_tropa, int x_, int y_);

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
    int x, y;
};

} // namespace conexion

#endif // _EVENTO_SINCRONIZAR_TROPA_H_
