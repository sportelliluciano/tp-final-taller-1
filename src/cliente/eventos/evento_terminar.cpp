#include "cliente/eventos/evento_terminar.h"

#include "cliente/modelo/juego.h"

namespace cliente {

void EventoTerminar::actualizar(Juego& juego) {
    juego.detener();
}

} // namespace cliente