#include "conexion/eventos/evento_mostrar_gusano.h"

#include "cliente/modelo/juego.h"

namespace conexion {

EventoMostrarGusano::EventoMostrarGusano(const nlohmann::json& serializado) {
    x = serializado.at("x");
    y = serializado.at("y");
}

EventoMostrarGusano::EventoMostrarGusano(int x_, int y_)
    : x(x_), y(y_)
{ }

void EventoMostrarGusano::actualizar(cliente::Juego& juego) {
    juego.mostrar_gusano(x, y);
}

nlohmann::json EventoMostrarGusano::serializar() const {
    return {
        {"id", EV_MOSTRAR_GUSANO},
        {"x", x},
        {"y", y}
    };
}

} // namespace conexion