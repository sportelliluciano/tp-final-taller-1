#include "cliente/modelo/eventos/evento_jugador.h"

#include "libs/json.hpp"

#include "comun/eventos_cliente.h"
#include "cliente/modelo/juego.h"

namespace cliente {

EventoJugador::EventoJugador(const nlohmann::json& serializado) {
    data = serializado;
}

void EventoJugador::actualizar(Juego& juego) {
    evento_cliente_t id_evento = (evento_cliente_t)data.at("id").get<int>();
    switch(id_evento) {
        case EVC_INICIALIZAR:
            juego.inicializar(data.at("id_jugador"), data.at("edificios"),
                data.at("ejercitos"), data.at("mapa"));
            break;
        case EVC_CREAR_JUGADOR:
            juego.crear_jugador(data.at("id_jugador"), data.at("nombre"), 
                data.at("casa"));
            break;
        case EVC_JUGADOR_LISTO:
            juego.indicar_jugador_listo(data.at("id_jugador"));
            break;
        case EVC_JUEGO_INICIANDO:
            juego.sincronizar_inicio();
            break;
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