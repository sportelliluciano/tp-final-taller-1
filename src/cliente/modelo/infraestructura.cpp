#include "cliente/modelo/infraestructura.h"

#include "cliente/modelo/terreno.h"

namespace cliente {

Infraestructura::Infraestructura(Terreno& terreno_juego) 
: terreno(terreno_juego)
{ }

void Infraestructura::renderizar(Ventana& ventana) {
    for (auto it = edificios.begin(); it != edificios.end(); ++it) {
        Edificio* edificio = *it;
        if (terreno.esta_en_camara(edificio->obtener_celdas_ocupadas(), 
            ventana)) {
            edificio->renderizar(ventana);
        }
    }
}

} // namespace cliente
