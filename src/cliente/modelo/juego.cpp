#include "cliente/modelo/juego.h"

#include <string>

#include "libs/json.hpp"

#include "cliente/modelo/ejercito.h"
#include "cliente/modelo/gusano_arena.h"
#include "cliente/modelo/infraestructura.h"
#include "cliente/modelo/terreno.h"
#include "cliente/video/ventana.h"

namespace cliente {

Juego::Juego(int id_jugador_actual_, 
        const nlohmann::json& mapa,
        const nlohmann::json& edificios,
        const nlohmann::json& ejercito)
: terreno(mapa), 
  gusano(terreno), 
  infraestructura(id_jugador_actual_, terreno, edificios), 
  ejercito(id_jugador_actual_, terreno, ejercito), 
  esta_jugando(true), 
  id_jugador_actual(id_jugador_actual_)
{ }

bool Juego::esta_terminado() const {
    return !esta_jugando;
}

void Juego::renderizar(Ventana& ventana, Camara& camara) {
    terreno.renderizar(ventana, camara);
    gusano.renderizar(ventana, camara);
    infraestructura.renderizar(ventana, camara);
    ejercito.renderizar(ventana, camara);
}

void Juego::actualizar(int t_ms) {
    infraestructura.actualizar(t_ms);
    ejercito.actualizar(t_ms);
}

void Juego::detener() {
    esta_jugando = false;
}

int Juego::obtener_dinero() const {
    return dinero;
}

Posicion Juego::obtener_centro() {
    Edificio* centro = infraestructura.obtener_centro_construccion();
    if (!centro)
        return Posicion(0, 0);
    return terreno.obtener_posicion(centro);
}

Infraestructura& Juego::obtener_infraestructura() {
    return infraestructura;
}

Ejercito& Juego::obtener_ejercito() {
    return ejercito;
}

Terreno& Juego::obtener_terreno() {
    return terreno;
}

/***** Eventos recibidos desde el servidor *****/

void Juego::actualizar_dinero(int nuevo_dinero, int) {
    dinero = nuevo_dinero;
}

void Juego::actualizar_energia(int nueva_energia, int) {
    energia = nueva_energia;
}

void Juego::mostrar_gusano(int x, int y) {
    gusano.aparecer(x, y);
}

Juego::~Juego() { }

} // namespace cliente
