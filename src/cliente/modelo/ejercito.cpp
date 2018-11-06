#include "cliente/modelo/ejercito.h"

#include "cliente/modelo/terreno.h"

namespace cliente {

Ejercito::Ejercito(Terreno& terreno_juego) : terreno(terreno_juego) { }

void Ejercito::renderizar(Ventana& ventana) {
    tropa.renderizar(ventana);
}

} // namespace cliente
