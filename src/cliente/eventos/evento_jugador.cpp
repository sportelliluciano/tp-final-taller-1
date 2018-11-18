#include "cliente/eventos/evento_jugador.h"

#include "libs/json.hpp"

#include "conexion/eventos_cliente.h"
#include "cliente/modelo/juego.h"

namespace cliente {

EventoJugador::EventoJugador(const nlohmann::json& serializado) {
    data = serializado;
}

void EventoJugador::actualizar(Juego& juego) {
    using namespace conexion;
    evento_cliente_t id_evento = (evento_cliente_t)data.at("id").get<int>();
    switch(id_evento) {
        case EVC_ACTUALIZAR_DINERO:        // (nuevo_dinero, nuevo_maximo)
            juego.actualizar_dinero(data.at("nuevo_dinero"), 
                data.at("nuevo_maximo"));
            break;
        case EVC_ACTUALIZAR_ENERGIA:       // (nueva_energia, nuevo_maximo)
            juego.actualizar_energia(data.at("nueva_energia"), 
                data.at("nuevo_maximo"));
            break;
        default:
            // Tirar excepcion
            break;
    }
}

} // namespace cliente