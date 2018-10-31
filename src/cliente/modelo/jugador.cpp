#include "cliente/modelo/jugador.h"

namespace cliente {

Jugador::Jugador(Terreno& terreno_juego) : terreno(terreno_juego),
    infraestructura(terreno), tropas(terreno) { }

void Jugador::construir_edificio(const std::string& edificio, int x, int y) {
    infraestructura.construir(edificio, x, y);
}

void Jugador::renderizar(Ventana& ventana) {
    infraestructura.renderizar(ventana);
    tropas.renderizar(ventana);
}

} // namespace cliente