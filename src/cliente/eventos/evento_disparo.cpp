#include "cliente/eventos/evento_disparo.h"

#include "libs/json.hpp"

#include "conexion/eventos_cliente.h"
#include "cliente/modelo/juego.h"

namespace cliente {

EventoDisparo::EventoDisparo(const nlohmann::json& serializado) {
    data = serializado;
}

void EventoDisparo::actualizar(Juego&) {
    using namespace conexion;
    evento_cliente_t id_evento = (evento_cliente_t)data.at("id").get<int>();
    switch(id_evento) {
        case EVC_LANZAR_MISIL:             // (id, pos_inicio, pos_fin)
            break;
        case EVC_ONDA_SONIDO:              // (id, pos_inicio, pos_fin)
            break;
        case EVC_LANZAR_PLASMA:            // (id, pos_inicio, pos_fin)
            break;
        case EVC_SINCRONIZAR_DISPARO:      // (id, pos_actual, pos_fin)
            break;
        default:
            // Tirar excepcion
            break;
    }
}

} // namespace cliente