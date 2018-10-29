#include "cliente/modelo/juego.h"

#include "cliente/modelo/terreno.h"

namespace cliente {

Juego::Juego(Ventana& ventana_juego, const char* edificio) 
    : ventana(ventana_juego) {
    terreno = new Terreno("../data/terreno.csv");
    jugador = new Jugador(*terreno);
    jugador->construir_edificio(edificio);
}

bool Juego::esta_terminado() const {
    return !esta_jugando;
}

void Juego::actualizar() {
    terreno->renderizar(ventana);
    jugador->renderizar(ventana);
}

void Juego::detener() {
    esta_jugando = false;
}

Juego::~Juego() {
    delete terreno;
}

} // namespace cliente
