#include "cliente/eventos/factory_evento.h"

#include "conexion/error_conexion.h"
#include "conexion/eventos_cliente.h"
#include "cliente/eventos/evento_ejercito.h"
#include "cliente/eventos/evento_infraestructura.h"

namespace cliente {

Evento* FactoryEvento::crear_desde_json(const nlohmann::json& serializado) {
    using namespace conexion;
    if (serializado.find("id") == serializado.end())
        throw ErrorConexion("El JSON no contiene ID");
    
    evento_cliente_t id_evento = serializado.at("id");

    switch(id_evento) {
        case EVC_INICIAR_CONSTRUCCION:
        case EVC_SINCRONIZAR_CONSTRUCCION:
        case EVC_SET_VELOCIDAD_CC:
        case EVC_ACTUALIZAR_COLA_CC:
        case EVC_ATACAR_EDIFICIO:
        case EVC_CREAR_EDIFICIO:
        case EVC_AGREGAR_EDIFICIO:
        case EVC_ELIMINAR_EDIFICIO:
        case EVC_DESTRUIR_EDIFICIO:
            return new EventoInfraestructura(serializado);
        case EVC_ENTRENAR_TROPA:
        case EVC_SINCRONIZAR_ENTRENAMIENTO:
        case EVC_ACTUALIZAR_COLA_EE:
        case EVC_CREAR_TROPA:
        case EVC_MOVER_TROPA:
        case EVC_SINCRONIZAR_TROPA:
        case EVC_ATACAR_TROPA:
        case EVC_DESTRUIR_TROPA:
            return new EventoEjercito(serializado);
        default:
            break;
    }
    throw ErrorConexion("Evento desconocido");
}

} // namespace cliente
