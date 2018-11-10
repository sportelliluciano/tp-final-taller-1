#include "conexion/eventos/evento_mover_tropa.h"

#include <iostream>
#include <vector>

#include "libs/json.hpp"

#include "cliente/modelo/juego.h"

namespace conexion {

EventoMoverTropa::EventoMoverTropa(const nlohmann::json& serializado) {
    tropa = serializado.at("id_tropa");
    pasos = serializado.at("camino").get<std::vector<int>>();

    if (pasos.size() % 2 != 0)
        throw std::runtime_error("Camino invalido (pasos.size() % 2 != 0)");
}

EventoMoverTropa::EventoMoverTropa(int id_tropa, const std::vector<int>& camino)
    : tropa(id_tropa), pasos(camino)
{ }

void EventoMoverTropa::actualizar(cliente::Juego& juego) {
    std::vector<std::pair<int, int>> camino;
    for (size_t i=0;i<pasos.size();i+=2) {
        std::pair<int, int> paso = {pasos[i], pasos[i+1]};
        camino.emplace_back(paso);
    }
    
    juego.indicar_camino_tropa(tropa, camino);
}

nlohmann::json EventoMoverTropa::serializar() const {
    return {
        {"id", EV_MOVER_TROPA},
        {"id_tropa", tropa},
        {"camino", pasos}
    };
}

} // namespace conexion