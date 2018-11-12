#include "cliente/eventos/evento_ejercito.h"

#include "libs/json.hpp"

#include "conexion/eventos_cliente.h"
#include "cliente/modelo/juego.h"

namespace cliente {

EventoEjercito::EventoEjercito(const nlohmann::json& serializado) {
    data = serializado;
}

void EventoEjercito::actualizar(Juego& juego) {
    using namespace conexion;
    Ejercito& ejercito = juego.obtener_ejercito();
    evento_cliente_t id_evento = (evento_cliente_t)data.at("id").get<int>();
    switch(id_evento) {
        case EVC_ENTRENAR_TROPA:           // (clase, tiempo)
            ejercito.entrenar(data.at("clase"), data.at("tiempo"));
            break;
        case EVC_SINCRONIZAR_ENTRENAMIENTO: // (clase, tiempo)
            ejercito.sincronizar_entrenamiento(data.at("clase"), 
                data.at("tiempo"));
            break;
        case EVC_ACTUALIZAR_COLA_EE:       // (clase, cantidad)
            ejercito.actualizar_cola_entrenamiento(data.at("clase"), 
                data.at("cantidad"));
            break;
        case EVC_CREAR_TROPA:              // (id, clase, posicion_inicial, id_jugador)
            ejercito.crear_tropa(data.at("id_tropa"), data.at("clase"), 
                data.at("posicion"), data.at("id_jugador"));
            break;
        case EVC_MOVER_TROPA:              // (id, camino)
            ejercito.mover_tropa(data.at("id_tropa"), data.at("camino"));
            break;
        case EVC_SINCRONIZAR_TROPA:        // (id, posición actual)
            ejercito.sincronizar_tropa(data.at("id_tropa"), data.at("posicion"));
            break;
        case EVC_ATACAR_TROPA:             // (id, nueva_vida)
            ejercito.atacar_tropa(data.at("id_tropa"), data.at("nueva_vida"));
            break;
        case EVC_DESTRUIR_TROPA:           // (id)
            ejercito.destruir_tropa(data.at("id_tropa"));
            break;
        default:
            // Tirar excepcion
            break;
    }
}

} // namespace cliente