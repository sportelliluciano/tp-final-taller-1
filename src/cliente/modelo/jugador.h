#ifndef _JUGADOR_H_
#define _JUGADOR_H_

#include <string>

#include "cliente/modelo/ejercito.h"
#include "cliente/modelo/infraestructura.h"
#include "cliente/modelo/terreno.h"
#include "cliente/modelo/hud.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Jugador {
public:
    Jugador(Terreno& terreno_juego);
    void renderizar(Ventana& ventana);
    void construir_edificio(int id, const std::string& clase, int x, int y);
    void destruir_edificio(int id);
private:
    Terreno& terreno;
    int energia = 0;
    int dinero = 0;
    Infraestructura infraestructura;
    Ejercito tropas;
};

} // namespace cliente

#endif // _JUGADOR_H_
