#include "conexion/factory_evento.h"

#include "conexion/error_conexion.h"
#include "conexion/eventos/evento_crear_edificio.h"

namespace conexion {

Evento* FactoryEvento::crear_desde_json(const nlohmann::json& serializado) {
    if (serializado.find("id") == serializado.end())
        throw ErrorConexion("El JSON no contiene ID");
    
    evento_t id_evento = serializado["id"];

    switch(id_evento) {
        case EV_CREAR_EDIFICIO:
            return new EventoCrearEdificio(serializado);
        default:
            throw ErrorConexion("Evento desconocido");
    }
}

} // namespace conexion
