#include "cliente/modelo/juego.h"

#include <string>

#include "cliente/modelo/gusano_arena.h"
#include "cliente/modelo/jugador.h"
#include "cliente/modelo/terreno.h"
#include "cliente/video/ventana.h"

namespace cliente {

Juego::Juego(const char* edificio) {
    terreno = new Terreno("../data/terreno.csv");
    jugador = new Jugador(*terreno);
    gusano = new GusanoArena(*terreno);
}

bool Juego::esta_terminado() const {
    return !esta_jugando;
}

void Juego::renderizar(Ventana& ventana) {
    terreno->renderizar(ventana);
    gusano->renderizar(ventana);
    jugador->renderizar(ventana);
}

int Juego::obtener_dinero() const {
    return jugador->obtener_dinero();
}

void Juego::setear_dinero(int dinero) {
    jugador->setear_dinero(dinero);
}

void Juego::construir_edificio(int id, const std::string& clase, int x, int y) {
    jugador->construir_edificio(id, clase, x, y);
}

void Juego::destruir_edificio(int id) {
    jugador->destruir_edificio(id);
}

void Juego::mostrar_gusano(int x, int y) {
    gusano->aparecer(x, y);
}

void Juego::detener() {
    esta_jugando = false;
}

Juego::~Juego() {
    delete terreno;
}

} // namespace cliente
