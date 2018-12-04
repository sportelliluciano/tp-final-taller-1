#include "servidor/cliente.h"

#include <mutex>
#include <string>
#include <thread>

#include "libs/json.hpp"

#include "comun/conexion.h"
#include "comun/lock.h"
#include "comun/log.h"
#include "comun/socket_conexion.h"
#include "comun/cola_protegida.h"

namespace servidor {

Cliente::Cliente(SocketConexion socket_conexion) 
: conexion(socket_conexion), nombre("-")
{ }

Cliente::Cliente(Cliente&& otro) 
: conexion(std::move(otro.conexion))
{
    if (async_iniciado) {
        // rollback
        otro.conexion = std::move(conexion);
        throw std::runtime_error("No se puede mover habiendo iniciado el async");
    }

    cb_al_recibir_datos = otro.cb_al_recibir_datos;
    
    // TODO: Revisar esto
    while (!otro.cola_salida.esta_vacia())
        cola_salida.push(otro.cola_salida.pull());
    
    detencion_solicitada = otro.detencion_solicitada;
    hubo_error_emisor = otro.hubo_error_emisor;
    hubo_error_receptor = otro.hubo_error_receptor;
    error_emisor = std::move(otro.error_emisor);
    error_receptor = std::move(otro.error_receptor);

    async_iniciado = false;
}

void Cliente::main_hilo_emisor() { 
    try {
        while (conexion.esta_conectada()) {
            conexion.enviar_json(cola_salida.pull());
        }
    } catch(const std::exception& e) {
        error_emisor = e.what();
        hubo_error_emisor = true;
        conexion.cerrar(true);
        if (!detencion_solicitada)
            log_advertencia("Hilo cliente (E) explotó\n --> %s", e.what());
    }

    log_depuracion("Hilo cliente (E) terminado");
}

void Cliente::main_hilo_receptor() { 
    try {
        while (conexion.esta_conectada()) {
            nlohmann::json data = conexion.recibir_json();
            {
                Lock l(m_cb_al_recibir_datos);
                if (cb_al_recibir_datos)
                    cb_al_recibir_datos(data);
            }
        }
    } catch(const std::exception& e) {
        error_receptor = e.what();
        hubo_error_receptor = true;
        conexion.cerrar(true);
        if (!detencion_solicitada)
            log_advertencia("Hilo cliente (R) explotó\n --> %s", e.what());
    }

    log_depuracion("Hilo cliente (R) terminado");
    cola_salida.desbloquear();
}

void Cliente::iniciar_async() {
    if (async_iniciado)
        throw std::runtime_error("Este cliente ya fue iniciado");
    
    async_iniciado = true;
    hilo_emisor = std::thread(&Cliente::main_hilo_emisor, this);
    hilo_receptor = std::thread(&Cliente::main_hilo_receptor, this);
}

void Cliente::detener_async() {
    detencion_solicitada = true;
    conexion.cerrar(true);
    cola_salida.desbloquear();
    if (hilo_emisor.joinable())
        hilo_emisor.join();
    if (hilo_receptor.joinable())
        hilo_receptor.join();
}

void Cliente::enviar(const nlohmann::json& data) {
    cola_salida.push(data);
}

void Cliente::al_recibir_datos(
    std::function<void(const nlohmann::json& data)> callback) 
{
    Lock l(m_cb_al_recibir_datos);
    cb_al_recibir_datos = callback;
}

bool Cliente::esta_conectado() const {
    return conexion.esta_conectada();
}

bool Cliente::hubo_error() const {
    if (detencion_solicitada)
        return false;
    
    return hubo_error_emisor || hubo_error_receptor;
}

const std::string& Cliente::obtener_error() const {
    if (hubo_error_emisor)
        return error_emisor;
    
    return error_receptor;
}

void Cliente::set_nombre(const std::string& nuevo_nombre) {
    nombre = nuevo_nombre;
}

const std::string& Cliente::obtener_nombre() const {
    return nombre;
}

} // namespace servidor