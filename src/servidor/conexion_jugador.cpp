#include "conexion_jugador.h"

#include <iostream>

#include "comun/conexion.h"
#include "comun/eventos_cliente.h"
#include "comun/i_jugador.h"
#include "comun/i_modelo.h"
#include "servidor/cliente.h"

#define SIN_IMPLEMENTAR(evento) { \
    std::cout << "\x1b[33m[IJugador] ADV\x1b[0m: " \
              << "Evento " evento " no implementado" \
              << std::endl; \
}

namespace servidor {

ConexionJugador::ConexionJugador(Cliente& cliente, int id_)
: conexion_cliente(cliente), id(id_)
{ }

void ConexionJugador::set_estado(bool listo_) {
    listo = listo_;
}
bool ConexionJugador::esta_listo() const {
    return listo;
}

int ConexionJugador::obtener_id() const {
    return id;
}

const std::string& ConexionJugador::obtener_casa() const {
    return casa;
}

const std::string& ConexionJugador::obtener_nombre() const {
    return nombre;
}

void ConexionJugador::notificar(const nlohmann::json& data) {
    conexion_cliente.enviar(data);
}

ConexionJugador::~ConexionJugador() {

}

/***** Implementaciones de mensajes *****/

void ConexionJugador::inicializar(const nlohmann::json& mapa, 
    const nlohmann::json& infraestructura,
    const nlohmann::json& ejercito) 
{
    notificar({
        {"id", EVC_INICIALIZAR_EJERCITO},
        {"ejercito", ejercito}
    });
}

void ConexionJugador::juego_iniciando() {
    notificar({
        {"id", EVC_JUEGO_INICIANDO},
        {"id_jugador", id}
    });
}

void ConexionJugador::iniciar_construccion(const std::string& clase, int tiempo)
{
    notificar({
        {"id", EVC_INICIAR_CONSTRUCCION},
        {"clase", clase},
        {"tiempo", tiempo}
    });
}

void ConexionJugador::sincronizar_construccion(const std::string& clase, int ms)
{
    notificar({
        {"id", EVC_SINCRONIZAR_CONSTRUCCION},
        {"clase", clase},
        {"tiempo", ms}
    });
}

void ConexionJugador::set_velocidad_cc(float factor) {
    notificar({
        {"id", EVC_SET_VELOCIDAD_CC},
        {"velocidad", factor}
    });
}

void ConexionJugador::actualizar_cola_cc(const std::string& clase, int cantidad)
{
    notificar({
        {"id", EVC_ACTUALIZAR_COLA_CC},
        {"clase", clase},
        {"cantidad", cantidad}
    });
}

void ConexionJugador::crear_edificio(int id_edificio, const std::string& clase, 
    int celda_x, int celda_y, int id_propietario)
{
    notificar({
        {"id", EVC_CREAR_EDIFICIO},
        {"clase", clase},
        {"posicion", {celda_x, celda_y}},
        {"id_jugador", id_propietario},
        {"id_edificio", id_edificio}
    });
}

void ConexionJugador::destruir_edificio(int id_edificio) {
    notificar({
        {"id", EVC_DESTRUIR_EDIFICIO},
        {"id_edificio", id_edificio}
    });
}

void ConexionJugador::agregar_edificio(int id_edificio, 
    const std::string& clase, int celda_x, int celda_y, int id_propietario, 
    int vida)
{
    notificar({
        {"id", EVC_AGREGAR_EDIFICIO},
        {"id_edificio", id_edificio},
        {"clase", clase},
        {"posicion", {celda_x, celda_y}},
        {"id_jugador", id_propietario},
        {"vida", vida}
    });
}

void ConexionJugador::eliminar_edificio(int id_edificio) {
    notificar({
        {"id", EVC_ELIMINAR_EDIFICIO},
        {"id_edificio", id_edificio}
    });
}

void ConexionJugador::iniciar_entrenamiento(const std::string& clase, int ms) {
    notificar({
        {"id", EVC_ENTRENAR_TROPA},
        {"clase", clase},
        {"tiempo", ms}
    });
}

void ConexionJugador::sincronizar_entrenamiento(const std::string& clase, 
    int ms) 
{
    notificar({
        {"id", EVC_SINCRONIZAR_ENTRENAMIENTO},
        {"clase", clase},
        {"tiempo", ms}
    });
}

void ConexionJugador::actualizar_cola_ee(const std::string& clase, int cantidad)
{
    notificar({
        {"id", EVC_ACTUALIZAR_COLA_EE},
        {"clase", clase},
        {"cantidad", cantidad}
    });
}

void ConexionJugador::crear_tropa(int id_tropa, const std::string& clase, 
    int pos_x, int pos_y, int vida, int id_propietario)
{
    notificar({
        {"id", EVC_CREAR_TROPA},
        {"id_tropa", id_tropa},
        {"clase", clase},
        {"posicion", {pos_x, pos_y}},
        {"vida", vida},
        {"id_jugador", id_propietario},
    });
}

void ConexionJugador::mover_tropa(int id_tropa, 
    const std::vector<std::pair<int, int>>& camino) 
{
    std::vector<int> camino_aplanado;

    for (const std::pair<int, int>& paso : camino) {
        camino_aplanado.push_back(paso.first);
        camino_aplanado.push_back(paso.second);
    }

    notificar({
        {"id", EVC_MOVER_TROPA},
        {"id_tropa", id_tropa},
        {"camino", camino_aplanado}
    });
}

void ConexionJugador::sincronizar_tropa(int id_tropa, int pos_x, int pos_y) {
    notificar({
        {"id", EVC_SINCRONIZAR_TROPA},
        {"id_tropa", id_tropa},
        {"posicion", {pos_x, pos_y}}
    });
}

void ConexionJugador::atacar(int id_atacante, int id_victima, int nueva_vida) {
    notificar({
        {"id", EVC_ATACAR},
        {"id_atacante", id_atacante},
        {"id_victima", id_victima},
        {"nueva_vida", nueva_vida}
    });
}

void ConexionJugador::destruir_tropa(int id_tropa) {
    notificar({
        {"id", EVC_DESTRUIR_TROPA},
        {"id_tropa", id_tropa}
    });
}

void ConexionJugador::cosechadora_descargar(int id_tropa) {
    notificar({
        {"id", EVC_COSECHADORA_DESCARGAR},
        {"id_tropa", id_tropa}
    });
}

void ConexionJugador::mostrar_gusano(int celda_x, int celda_y) {
    notificar({
        {"id", EVC_MOSTRAR_GUSANO},
        {"posicion", {celda_x, celda_y}}
    });
}

void ConexionJugador::actualizar_dinero(int nuevo_dinero, int nuevo_maximo) {
    notificar({
        {"id", EVC_ACTUALIZAR_DINERO},
        {"nuevo_dinero", nuevo_dinero},
        {"nuevo_maximo", nuevo_maximo},
    });
}

void ConexionJugador::actualizar_energia(int nueva_energia, int nuevo_maximo) {
    notificar({
        {"id", EVC_ACTUALIZAR_ENERGIA},
        {"nueva_energia", nueva_energia},
        {"nuevo_maximo", nuevo_maximo},
    });
}

/***** Eventos no implementados *****/

void ConexionJugador::eliminar_especia(int, int) {
    SIN_IMPLEMENTAR("eliminar_especia");
}

void ConexionJugador::crear_jugador(int id_jugador, const std::string& nombre_, 
    const std::string& casa_)
{
    notificar({
        {"id", EVC_CREAR_JUGADOR},
        {"id_jugador", id_jugador},
        {"nombre", nombre_},
        {"casa", casa_}
    });
}

void ConexionJugador::jugador_listo(int id_jugador) {
    notificar({
        {"id", EVC_JUGADOR_LISTO},
        {"id_jugador", id_jugador}
    });
}

void ConexionJugador::juego_terminado(int id_ganador) {
    notificar({
        {"id", EVC_JUEGO_TERMINADO},
        {"id_ganador", id_ganador}
    });
}

} // namespace servidor
