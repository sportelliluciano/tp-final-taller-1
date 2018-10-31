#include "conexion/eventos/evento_crear_edificio.h"

#include "cliente/modelo/juego.h"

namespace conexion {

EventoCrearEdificio::EventoCrearEdificio(const nlohmann::json& serializado) {
    clase = serializado["clase"];
    x = serializado["x"];
    y = serializado["y"];
}

EventoCrearEdificio::EventoCrearEdificio(
    const std::string& clase_, int x_, int y_)
    : clase(clase_), x(x_), y(y_)
{ }

void EventoCrearEdificio::actualizar(cliente::Juego& juego) {
    juego.construir_edificio(clase, x, y);
}

nlohmann::json EventoCrearEdificio::serializar() const {
    return {
        {"id", EV_CREAR_EDIFICIO},
        {"clase", clase},
        {"x", x},
        {"y", y}
    };
}

} // namespace conexion