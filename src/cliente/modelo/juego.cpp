#include "cliente/modelo/juego.h"

#include <string>

#include "libs/json.hpp"

#include "cliente/modelo/ejercito.h"
#include "cliente/modelo/gusano_arena.h"
#include "cliente/modelo/infraestructura.h"
#include "cliente/modelo/terreno.h"
#include "cliente/video/ventana.h"

namespace cliente {

Juego::Juego(const std::string& casa_)
: esta_jugando(true), 
  casa(casa_)
{ }

void Juego::inicializar(int id_jugador, const nlohmann::json& edificios, 
    const nlohmann::json& ejercitos, const nlohmann::json& mapa)
{
    terreno = new Terreno(mapa);
    infraestructura = new Infraestructura(id_jugador, *terreno, edificios);
    ejercito = new Ejercito(ejercitos, *infraestructura, *terreno, id_jugador, 
        casa);
    gusano = new GusanoArena(*terreno);
    inicializado = true;
}

void Juego::sincronizar_inicio() {
    sincronizado = true;
}

bool Juego::esta_terminado() const {
    return !esta_jugando;
}

void Juego::renderizar(Ventana& ventana, Camara& camara) {
    terreno->renderizar(ventana, camara);
    gusano->renderizar(ventana, camara);
    infraestructura->renderizar(ventana, camara);
    ejercito->renderizar(ventana, camara);
}

void Juego::actualizar(int t_ms) {
    infraestructura->actualizar(t_ms);
    ejercito->actualizar(t_ms);
}

void Juego::detener() {
    esta_jugando = false;
}

int Juego::obtener_dinero() const {
    return dinero;
}

int Juego::obtener_energia() const {
    return energia;
}

const std::string& Juego::obtener_casa_jugador() const {
    return casa;
}

Posicion Juego::obtener_centro() {
    Edificio* centro = infraestructura->obtener_centro_construccion();
    if (!centro)
        return Posicion(0, 0);
    return terreno->obtener_posicion(centro);
}

Infraestructura& Juego::obtener_infraestructura() {
    return *infraestructura;
}

Ejercito& Juego::obtener_ejercito() {
    return *ejercito;
}

Terreno& Juego::obtener_terreno() {
    return *terreno;
}

/***** Eventos recibidos desde el servidor *****/

void Juego::actualizar_dinero(int nuevo_dinero, int) {
    dinero = nuevo_dinero;
}

void Juego::actualizar_energia(int nueva_energia, int) {
    energia = nueva_energia;
}

void Juego::mostrar_gusano(int x, int y) {
    gusano->aparecer(x, y);
}

void Juego::terminar(const std::string& nombre_ganador) {
    ganador = nombre_ganador;
    esta_jugando = false;
    termino_ok = true;
}

bool Juego::inicio_sincronizado() const {
    return sincronizado;
}

bool Juego::inicializacion_completa() const {
    return inicializado;
}

void Juego::crear_jugador(int id_jugador, const std::string& nombre, 
        const std::string& casa)
{

}
    
void Juego::indicar_jugador_listo(int id_jugador) {

}

bool Juego::termino_correctamente() const {
    return termino_ok;
}

const std::string& Juego::obtener_ganador() const {
    return ganador;
}

Juego::~Juego() {
    delete gusano;
    delete ejercito;
    delete infraestructura;
    delete terreno;
}

} // namespace cliente
