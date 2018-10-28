#ifndef _JUGADOR_H_
#define _JUGADOR_H_

#include "cliente/modelo/ejercito.h"
#include "cliente/modelo/infraestructura.h"
#include "cliente/modelo/terreno.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Jugador {
public:
    Jugador(Terreno& terreno_juego);
    void renderizar(Ventana& ventana);

private:
    Terreno& terreno;
    int energia;
    int dinero;
    Infraestructura infraestructura;
    Ejercito tropas;
};

} // namespace cliente

#endif // _JUGADOR_H_
