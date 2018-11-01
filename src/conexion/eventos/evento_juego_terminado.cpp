#include "conexion/eventos/evento_juego_terminado.h"

#include "libs/json.hpp"

#include "cliente/modelo/juego.h"

namespace conexion {

EventoJuegoTerminado::EventoJuegoTerminado(const nlohmann::json&) {
}

void EventoJuegoTerminado::actualizar(cliente::Juego& juego) {
    juego.detener();
}

nlohmann::json EventoJuegoTerminado::serializar() const {
    return { {"id", EV_JUEGO_TERMINADO} };
}

} // namespace conexion