#include "cliente/modelo/jugador.h"

namespace cliente {

Jugador::Jugador(Terreno& terreno_juego) : terreno(terreno_juego),
    infraestructura(terreno), tropas(terreno) { }

void Jugador::construir_edificio(const char *edificio) {
    if (edificio)
        infraestructura.construir(edificio, 9, 9);
}

void Jugador::renderizar(Ventana& ventana) {
    infraestructura.renderizar(ventana);
    tropas.renderizar(ventana);
}

} // namespace cliente