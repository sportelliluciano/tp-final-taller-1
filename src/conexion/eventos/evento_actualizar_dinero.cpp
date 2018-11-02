#include "conexion/eventos/evento_actualizar_dinero.h"

#include "libs/json.hpp"

#include "cliente/modelo/juego.h"

namespace conexion {

EventoActualizarDinero::EventoActualizarDinero(const nlohmann::json& serializado) {
    nuevo_dinero = serializado.at("dinero");
}

EventoActualizarDinero::EventoActualizarDinero(int dinero)
    : nuevo_dinero(dinero)
{ }

void EventoActualizarDinero::actualizar(cliente::Juego& juego) {
    juego.setear_dinero(nuevo_dinero);
}

nlohmann::json EventoActualizarDinero::serializar() const {
    return {
        {"id", EV_ACTUALIZAR_DINERO},
        {"dinero", nuevo_dinero}
    };
}

} // namespace conexion