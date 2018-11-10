#include "conexion/eventos/evento_sincronizar_tropa.h"

#include "libs/json.hpp"

#include "cliente/modelo/juego.h"

namespace conexion {

EventoSincronizarTropa::EventoSincronizarTropa(const nlohmann::json& serializado) {
    tropa = serializado.at("id_tropa");
    x = serializado.at("x");
    y = serializado.at("y");
}

EventoSincronizarTropa::EventoSincronizarTropa(int id_tropa, int x_, int y_)
    : tropa(id_tropa), x(x_), y(y_)
{ }

void EventoSincronizarTropa::actualizar(cliente::Juego& juego) {
    juego.sincronizar_tropa(tropa, x, y);
}

nlohmann::json EventoSincronizarTropa::serializar() const {
    return {
        {"id", EV_SINCRONIZAR_TROPA},
        {"id_tropa", tropa},
        {"x", x},
        {"y", y}
    };
}

} // namespace conexion