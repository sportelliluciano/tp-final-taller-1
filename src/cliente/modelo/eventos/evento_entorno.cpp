#include "cliente/modelo/eventos/evento_entorno.h"

#include "libs/json.hpp"

#include "comun/eventos_cliente.h"
#include "cliente/modelo/juego.h"

namespace cliente {

EventoEntorno::EventoEntorno(const nlohmann::json& serializado) {
    data = serializado;
}

void EventoEntorno::actualizar(Juego& juego) {
    evento_cliente_t id_evento = (evento_cliente_t)data.at("id").get<int>();
    switch(id_evento) {
        case EVC_MOSTRAR_GUSANO:           // (posicion)
            juego.mostrar_gusano(
                (data.at("posicion").get<std::vector<int>>().at(0) * 8) / 32,
                (data.at("posicion").get<std::vector<int>>().at(1) * 8) / 32
            );
            break;
        case EVC_ELIMINAR_ESPECIA:         // (posicion)
            break;
        case EVC_COSECHADORA_DESCARGAR:    // (id)
            break;
        default:
            // Tirar excepcion
            break;
    }
}

} // namespace cliente
