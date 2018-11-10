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

void Juego::renderizar(Ventana& ventana) {
    terreno->renderizar(ventana);
    gusano->renderizar(ventana);
    jugador->renderizar(ventana);
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

void Juego::construir_edificio(int id, const std::string& clase, int x, int y) {
    jugador->construir_edificio(id, clase, x, y);
}

void Juego::destruir_edificio(int id) {
    jugador->destruir_edificio(id);
}

void Juego::crear_tropa(int id, const std::string& clase, int x, int y) {
    jugador->crear_tropa(id, clase, x, y);
}

void Juego::destruir_tropa(int id) {
    jugador->destruir_tropa(id);
}

void Juego::mostrar_gusano(int x, int y) {
    gusano->aparecer(x, y);
}

void Juego::detener() {
    esta_jugando = false;
}

void Juego::iniciar_construccion(const std::string& clase) {
    jugador->iniciar_construccion(clase);
}

std::vector<const Edificio*> Juego::obtener_edificios() const {
    return jugador->obtener_edificios();
}

void Juego::seleccionar_edificio(int x, int y) {
    const Edificio* edificio = terreno->obtener_edificio_en(x, y);

    if (edificio)
        jugador->seleccionar_edificio(*edificio);
    else
        jugador->deseleccionar_edificio();
}

void Juego::seleccionar_tropas(int x0, int y0, int x1, int y1) {
    jugador->seleccionar_unidades(
        terreno->seleccionar_unidades(x0, y0, x1, y1));
}

void Juego::sincronizar_tropa(int id_tropa, int x, int y) {
    jugador->sincronizar_tropa(id_tropa, x, y);
}

void Juego::indicar_camino_tropa(int id_tropa, 
    const std::vector<std::pair<int, int>>& camino) 
{
    jugador->indicar_camino_tropa(id_tropa, camino);
}

void Juego::mover_camara(int dx, int dy) {
    terreno->mover_camara(dx, dy);
}

Juego::~Juego() {
    delete gusano;
    delete jugador;
    delete terreno;
}

} // namespace cliente
