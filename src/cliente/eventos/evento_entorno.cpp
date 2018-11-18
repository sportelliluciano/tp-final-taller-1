#include "cliente/eventos/evento_entorno.h"

#include "libs/json.hpp"

#include "conexion/eventos_cliente.h"
#include "cliente/modelo/juego.h"

namespace cliente {

EventoEntorno::EventoEntorno(const nlohmann::json& serializado) {
    data = serializado;
}

void EventoEntorno::actualizar(Juego& juego) {
    using namespace conexion;
    evento_cliente_t id_evento = (evento_cliente_t)data.at("id").get<int>();
    switch(id_evento) {
        case EVC_MOSTRAR_GUSANO:           // (posicion)
            juego.mostrar_gusano(
                data.at("posicion").get<std::vector<int>>().at(0),
                data.at("posicion").get<std::vector<int>>().at(1)
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
