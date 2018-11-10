#ifndef _EVENTO_H_
#define _EVENTO_H_

#include "libs/json.hpp"

#include "cliente/modelo/juego.h"

namespace conexion {

typedef enum {
    EV_CREAR_EDIFICIO,
    EV_DESTRUIR_EDIFICIO,
    EV_MOSTRAR_GUSANO,
    EV_ACTUALIZAR_DINERO,
    EV_MOVER_TROPA,
    EV_SINCRONIZAR_TROPA,
    EV_JUEGO_TERMINADO
} evento_t;

/**
 * \brief Representa un evento recibido desde el servidor.
 */
class Evento {
public:
    /**
     * Todas las clases dependientes de evento deben implementar un constructor
     * que recibe un JSON serializado.
     * Evento(const nlohmann::json& serializado);
     * 
     * C++ no permite forzar la existencia de este constructor.
     */

    /**
     * \brief Aplica la actualización desde el servidor sobre el juego.
     */
    virtual void actualizar(cliente::Juego& juego) = 0;

    /**
     * \brief Serializa el evento en formato JSON
     */
    virtual nlohmann::json serializar() const = 0;

    /**
     * \brief Destructor virtual.
     */
    virtual ~Evento() { }
};

} // namespace conexion

#endif // _EVENTO_H_
