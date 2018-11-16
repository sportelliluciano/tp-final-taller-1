#include "cliente/modelo/jugador.h"

#include "cliente/modelo/ejercito.h"
#include "cliente/modelo/infraestructura.h"
#include "cliente/modelo/terreno.h"
#include "cliente/video/camara.h"
#include "cliente/video/ventana.h"

namespace cliente {

Jugador::Jugador(Terreno& terreno) 
: infraestructura(terreno), tropas(terreno) 
{ }

void Jugador::renderizar(Ventana& ventana, Camara& camara) {
    infraestructura.renderizar(ventana, camara);
    tropas.renderizar(ventana, camara);
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

Infraestructura& Jugador::obtener_infraestructura() {
    return infraestructura;
}

Ejercito& Jugador::obtener_ejercito() {
    return tropas;
}

} // namespace cliente