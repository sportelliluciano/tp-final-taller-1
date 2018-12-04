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

IJugador* Broadcaster::obtener_jugador(int id){
    for (auto it=jugadores.begin(); it!= jugadores.end(); ++it){
        if ((*it)->obtener_id() == id)
            return *it;
    }
}

} // namespace modelo
