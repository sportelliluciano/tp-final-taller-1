#ifndef _FACTORY_EVENTO_H_
#define _FACTORY_EVENTO_H_

#include "libs/json.hpp"

#include "cliente/modelo/eventos/evento.h"

namespace cliente {

/**
 * \brief Clase encargada de obtener un evento en formato JSON y 
 *        deserializarlo en una instancia del evento correcto.
 */
class FactoryEvento {
public:
    static Evento* crear_desde_json(const nlohmann::json& serializado);
};

} // namespace cliente

#endif // _FACTORY_EVENTO_H_
