#include "cliente/modelo/juego.h"

#include <string>

#include "cliente/modelo/gusano_arena.h"
#include "cliente/modelo/jugador.h"
#include "cliente/modelo/terreno.h"
#include "cliente/video/ventana.h"

namespace cliente {

Juego::Juego() {
    terreno = new Terreno("../data/terreno.csv");
    jugador = new Jugador(*terreno);
    gusano = new GusanoArena(*terreno);
}

bool Juego::esta_terminado() const {
    return !esta_jugando;
}

void Juego::renderizar(Ventana& ventana, Camara& camara) {
    terreno->renderizar(ventana, camara);
    gusano->renderizar(ventana, camara);
    jugador->renderizar(ventana, camara);
}

void Juego::actualizar(int t_ms) {
    jugador->actualizar(t_ms);
}

int Juego::obtener_dinero() const {
    return jugador->obtener_dinero();
}

void Juego::setear_dinero(int dinero) {
    jugador->setear_dinero(dinero);
}

void Juego::mostrar_gusano(int x, int y) {
    gusano->aparecer(x, y);
}

void Juego::detener() {
    esta_jugando = false;
}

Infraestructura& Juego::obtener_infraestructura() {
    return jugador->obtener_infraestructura();
}

Ejercito& Juego::obtener_ejercito() {
    return jugador->obtener_ejercito();
}

Terreno& Juego::obtener_terreno() {
    return *terreno;
}

Juego::~Juego() {
    delete gusano;
    delete jugador;
    delete terreno;
}

} // namespace cliente
