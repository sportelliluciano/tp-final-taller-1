#include "modelo/broadcaster.h"

#include <unordered_set>

#include "comun/i_jugador.h"

namespace modelo {

void Broadcaster::agregar_jugador(IJugador *nuevo_jugador) {
    jugadores.insert(nuevo_jugador);
}

void Broadcaster::eliminar_jugador(IJugador *jugador) {
    auto it = jugadores.find(jugador);
    if (it != jugadores.end())
        jugadores.erase(it);
}

void Broadcaster::broadcast(std::function<void(IJugador*)> mensaje) {
    for (IJugador *jugador : jugadores) {
        mensaje(jugador);
    }
}

} // namespace modelo
