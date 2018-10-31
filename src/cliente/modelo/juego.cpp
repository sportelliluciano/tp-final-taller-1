#include "cliente/modelo/juego.h"

#include <string>

#include "cliente/modelo/terreno.h"

namespace cliente {

Juego::Juego(const char* edificio) {
    terreno = new Terreno("../data/terreno.csv");
    jugador = new Jugador(*terreno);
}

bool Juego::esta_terminado() const {
    return !esta_jugando;
}

void Juego::renderizar(Ventana&  ventana) {
    terreno->renderizar(ventana);
    jugador->renderizar(ventana);
}

void Juego::construir_edificio(const std::string& clase, int x, int y) {
    jugador->construir_edificio(clase, x, y);
}

void Juego::detener() {
    esta_jugando = false;
}

Juego::~Juego() {
    delete terreno;
}

} // namespace cliente
