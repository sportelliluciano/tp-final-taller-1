#include "cliente/modelo/eventos/evento_terminar.h"

#include "cliente/modelo/juego.h"

namespace cliente {

EventoTerminar::EventoTerminar(const nlohmann::json& serializado) {
    data = serializado;
}

void EventoTerminar::actualizar(Juego& juego) {
    juego.terminar(data.at("ganador"));
}

} // namespace cliente