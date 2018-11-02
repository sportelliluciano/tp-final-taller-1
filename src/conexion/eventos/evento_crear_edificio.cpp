#include "conexion/eventos/evento_crear_edificio.h"

#include "libs/json.hpp"

#include "cliente/modelo/juego.h"

namespace conexion {

EventoCrearEdificio::EventoCrearEdificio(const nlohmann::json& serializado) {
    id = serializado.at("id_edificio");
    clase = serializado.at("clase");
    x = serializado.at("x");
    y = serializado.at("y");
}

EventoCrearEdificio::EventoCrearEdificio(
    int id_, const std::string& clase_, int x_, int y_)
    : id(id_), clase(clase_), x(x_), y(y_)
{ }

void EventoCrearEdificio::actualizar(cliente::Juego& juego) {
    juego.construir_edificio(id, clase, x, y);
}

nlohmann::json EventoCrearEdificio::serializar() const {
    return {
        {"id", EV_CREAR_EDIFICIO},
        {"id_edificio", id},
        {"clase", clase},
        {"x", x},
        {"y", y}
    };
}

} // namespace conexion