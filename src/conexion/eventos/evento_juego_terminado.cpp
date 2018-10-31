#include "conexion/eventos/evento_juego_terminado.h"

#include "cliente/modelo/juego.h"

namespace conexion {

EventoJuegoTerminado::EventoJuegoTerminado(const nlohmann::json&) {
}

void EventoJuegoTerminado::actualizar(cliente::Juego& juego) {
    juego.detener();
}

nlohmann::json EventoJuegoTerminado::serializar() const {
    return { };
}

} // namespace conexion