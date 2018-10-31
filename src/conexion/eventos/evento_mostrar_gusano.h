#ifndef _EVENTO_MOSTRAR_GUSANO_H_
#define _EVENTO_MOSTRAR_GUSANO_H_

#include "libs/json.hpp"

#include "conexion/evento.h"

namespace conexion {

class EventoMostrarGusano : public Evento {
public:
    /**
     * \brief Constructor por serialización.
     */
    EventoMostrarGusano(const nlohmann::json& serializado);

    /**
     * \brief Constructor.
     */
    EventoMostrarGusano(int x_, int y_);

    /**
     * \brief Actualiza el modelo del juego de un cliente.
     */
    void actualizar(cliente::Juego& juego);

    /**
     * \brief Devuelve una serialización del objeto en formato JSON.
     */
    nlohmann::json serializar() const;

private:
    int x, y;
};

} // namespace conexion

#endif // _EVENTO_MOSTRAR_GUSANO_H_
