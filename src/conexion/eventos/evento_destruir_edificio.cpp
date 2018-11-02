#include "conexion/eventos/evento_destruir_edificio.h"

#include "libs/json.hpp"

#include "cliente/modelo/juego.h"

namespace conexion {

EventoDestruirEdificio::EventoDestruirEdificio(const nlohmann::json& serializado) {
    id = serializado.at("id_edificio");
}

EventoDestruirEdificio::EventoDestruirEdificio(int id_)
    : id(id_)
{ }

void EventoDestruirEdificio::actualizar(cliente::Juego& juego) {
    juego.destruir_edificio(id);
}

nlohmann::json EventoDestruirEdificio::serializar() const {
    return {
        {"id", EV_DESTRUIR_EDIFICIO},
        {"id_edificio", id},
    };
}

} // namespace conexion