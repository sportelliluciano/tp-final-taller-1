#include "cliente/modelo/jugador.h"

namespace cliente {

Jugador::Jugador(Terreno& terreno_juego) : terreno(terreno_juego),
    infraestructura(terreno), tropas(terreno) { }

void Jugador::actualizar(int t_ms) {
    tropas.actualizar(t_ms);
    infraestructura.actualizar(t_ms);
}

void Jugador::construir_edificio(int id, const std::string& edificio, 
    int x, int y) {
    infraestructura.construir(id, edificio, x, y);
}

int Jugador::obtener_dinero() const {
    return dinero;
}

void Jugador::setear_dinero(int dinero_) {
    dinero = dinero_;
}

void Jugador::destruir_edificio(int id) {
    infraestructura.destruir(id);
}

void Jugador::crear_tropa(int id, const std::string& clase, int x, int y) {
    tropas.crear(id, clase, x, y);
}

void Jugador::destruir_tropa(int id) {
    tropas.destruir(id);
}

void Jugador::iniciar_construccion(const std::string& clase) {
    infraestructura.iniciar_construccion(clase);
}

std::vector<const Edificio*> Jugador::obtener_edificios() const {
    return infraestructura.obtener_edificios();
}

void Jugador::renderizar(Ventana& ventana) {
    infraestructura.renderizar(ventana);
    tropas.renderizar(ventana);
}

void Jugador::seleccionar_edificio(const Edificio& edificio) {
    infraestructura.seleccionar(edificio);
}

void Jugador::deseleccionar_edificio() {
    infraestructura.limpiar_seleccion();
}

void Jugador::seleccionar_unidades(
    const std::unordered_set<const Tropa*>& unidades) 
{
    tropas.seleccionar(unidades);
}

void Jugador::sincronizar_tropa(int id_tropa, int x, int y) {
    tropas.sincronizar_tropa(id_tropa, x, y);
}

void Jugador::indicar_camino_tropa(int id_tropa, 
    const std::vector<std::pair<int, int>>& camino) 
{
    tropas.indicar_camino_tropa(id_tropa, camino);
}

} // namespace cliente