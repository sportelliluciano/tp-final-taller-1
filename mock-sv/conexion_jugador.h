#ifndef _CONEXION_JUGADOR_H_
#define _CONEXION_JUGADOR_H_

#include <thread>

#include "../src/conexion/conexion.h"
#include "../src/conexion/eventos_cliente.h"
#include "cola.h"
#include "i_jugador.h"
#include "i_modelo.h"
#include "hilo_emisor.h"
#include "hilo_receptor.h"

using namespace conexion;

class ConexionJugador : public IJugador {
public:
    ConexionJugador(IModelo* modelo_, conexion::Conexion* conexion_, int id_)
    : hilo_emisor(*conexion_, cola_salida),
      hilo_receptor(*conexion_, modelo_, this)
    {
        id = id_;
        conexion = conexion_;
        modelo = modelo_;
        thread_he = std::thread(hilo_emisor);
        thread_hr = std::thread(hilo_receptor);
    }

    int obtener_id() const {
        return id;
    }

    void cerrar_conexion() {
        conexion->cerrar(true);
        thread_he.join();
        thread_hr.join();
    }

    void iniciar_construccion(const std::string& clase, int tiempo) {
        notificar({
            {"id", EVC_INICIAR_CONSTRUCCION},
            {"clase", clase},
            {"tiempo", tiempo}
        });
    }

    void sincronizar_construccion(const std::string& clase, int ms) {
        notificar({
            {"id", EVC_SINCRONIZAR_CONSTRUCCION},
            {"clase", clase},
            {"tiempo", ms}
        });
    }

    void set_velocidad_cc(float factor) {

    }

    void actualizar_cola_cc(const std::string& clase, int cantidad) {
        notificar({
            {"id", EVC_ACTUALIZAR_COLA_CC},
            {"clase", clase},
            {"cantidad", cantidad}
        });
    }

    void atacar_edificio(int id_edificio, int nueva_vida) {

    }

    void crear_edificio(int id_edificio, const std::string& clase, 
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

    void destruir_edificio(int id_edificio) {
        
    }

    void agregar_edificio(int id_edificio, const std::string& clase,
        int celda_x, int celda_y, int id_propietario, int vida)
    {
        
    }

    void eliminar_edificio(int id_edificio) {
        notificar({
            {"id", EVC_ELIMINAR_EDIFICIO},
            {"id_edificio", id_edificio}
        });
    }

    void iniciar_entrenamiento(const std::string& clase) {

    }

    void sincronizar_entrenamiento(const std::string& clase, int ms) {

    }

    void actualizar_cola_ee(const std::string& clase, int cantidad) {

    }

    void crear_tropa(int id_tropa, const std::string& clase, 
        int pos_x, int pos_y, int vida, int id_propietario)
    {
        notificar({
            {"id", EVC_CREAR_TROPA},
            {"clase", clase},
            {"id_tropa", id_tropa},
            {"id_jugador", id_propietario},
            {"posicion", {pos_x, pos_y}}
        });
    }

    void mover_tropa(int id_tropa, 
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

    void sincronizar_tropa(int id_tropa, int pos_x, int pos_y) {
        notificar({
            {"id", EVC_SINCRONIZAR_TROPA},
            {"id_tropa", id_tropa},
            {"posicion", {pos_x, pos_y}}
        });
    }

    void atacar_tropa(int id_tropa, int nueva_vida) {

    }

    void destruir_tropa(int id_tropa) {

    }

    void lanzar_misil() {

    }
    void onda_sonido() {

    }
    void lanzar_plasma() {

    }
    void sincronizar_disparo() {

    }

    void cosechadora_descargar(int id_tropa) {

    }

    void eliminar_especia() {

    }

    void mostrar_gusano(int celda_x, int celda_y) {

    }

    void actualizar_dinero(int nuevo_dinero, int nuevo_maximo) {

    }

    void actualizar_energia(int nueva_energia, int nuevo_maximo) {

    }

    void iniciar_juego() {

    }
    void crear_jugador() {

    }
    void jugador_listo() {

    }
    void juego_terminado() {

    }

private:
    conexion::Conexion* conexion;
    int id;
    Cola cola_entrada, cola_salida;
    IModelo* modelo;
    HiloEmisor hilo_emisor;
    HiloReceptor hilo_receptor;

    std::thread thread_he, thread_hr;

    void notificar(const nlohmann::json& data) {
        cola_salida.push(data);
    }
};

#endif // _CONEXION_JUGADOR_H_
