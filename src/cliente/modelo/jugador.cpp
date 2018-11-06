#include "cliente/modelo/jugador.h"

namespace cliente {

Jugador::Jugador(Terreno& terreno_juego) : terreno(terreno_juego),
    infraestructura(terreno), tropas(terreno) { }

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

} // namespace cliente