#include "cliente/modelo/jugador.h"

namespace cliente {

Jugador::Jugador(Terreno& terreno_juego) : terreno(terreno_juego),
    infraestructura(terreno), tropas(terreno) { }

void Jugador::renderizar(Ventana& ventana) {
    infraestructura.renderizar(ventana);
    tropas.renderizar(ventana);
}

void Jugador::actualizar(int t_ms) {
    tropas.actualizar(t_ms);
    infraestructura.actualizar(t_ms);
}

int Jugador::obtener_dinero() const {
    return dinero;
}

void Jugador::setear_dinero(int dinero_) {
    dinero = dinero_;
}

std::vector<const Edificio*> Jugador::obtener_edificios() const {
    return infraestructura.obtener_edificios();
}

Infraestructura& Jugador::obtener_infraestructura() {
    return infraestructura;
}

Ejercito& Jugador::obtener_ejercito() {
    return tropas;
}

} // namespace cliente