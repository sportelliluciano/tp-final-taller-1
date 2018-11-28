#include "cliente/eventos/factory_evento.h"

#include "comun/error_conexion.h"
#include "comun/eventos_cliente.h"
#include "cliente/eventos/evento_ejercito.h"
#include "cliente/eventos/evento_entorno.h"
#include "cliente/eventos/evento_infraestructura.h"
#include "cliente/eventos/evento_jugador.h"

namespace cliente {

Evento* FactoryEvento::crear_desde_json(const nlohmann::json& serializado) {
    if (serializado.find("id") == serializado.end())
        throw ErrorConexion("El JSON no contiene ID");
    
    evento_cliente_t id_evento = serializado.at("id");

    switch(id_evento) {
        case EVC_INICIAR_CONSTRUCCION:
        case EVC_SINCRONIZAR_CONSTRUCCION:
        case EVC_SET_VELOCIDAD_CC:
        case EVC_ACTUALIZAR_COLA_CC:
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
        case EVC_ATACAR:
        case EVC_DESTRUIR_TROPA:
            return new EventoEjercito(serializado);
        case EVC_ACTUALIZAR_DINERO:
        case EVC_ACTUALIZAR_ENERGIA:
            return new EventoJugador(serializado);
        case EVC_MOSTRAR_GUSANO:
        case EVC_ELIMINAR_ESPECIA:
        case EVC_COSECHADORA_DESCARGAR:
            return new EventoEntorno(serializado);
        default:
            break;
    }
    throw ErrorConexion("Evento desconocido");
}

} // namespace cliente
