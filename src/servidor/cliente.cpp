#include "servidor/cliente.h"

#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "libs/json.hpp"

#include "conexion/conexion.h"
#include "conexion/lock.h"
#include "conexion/socket_conexion.h"
#include "servidor/cola_protegida.h"

namespace servidor {

Cliente::Cliente(conexion::SocketConexion socket_conexion) 
: conexion(socket_conexion)
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

void Cliente::iniciar_async() {
    if (async_iniciado)
        throw std::runtime_error("Este cliente ya fue iniciado");
    
    async_iniciado = true;

    hilo_emisor = std::thread([this]() {
        try {
            while (conexion.esta_conectada()) {
                conexion.enviar_json(cola_salida.pull());
            }
        } catch(const std::exception& e) {
            error_emisor = e.what();
            hubo_error_emisor = true;
            conexion.cerrar(true);
            std::cout << "Hilo cliente (E) explotó" << std::endl;
            std::cout << " --> " << e.what() << std::endl;
        }

        std::cout << "Hilo cliente (E) died" << std::endl;
    });

    hilo_receptor = std::thread([this]() {
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
            std::cout << "Hilo cliente (R) explotó" << std::endl;
            std::cout << " --> " << e.what() << std::endl;
        }

        std::cout << "Hilo cliente (R) died" << std::endl;
    });
}

void Cliente::detener_async() {
    detencion_solicitada = true;
    conexion.cerrar(true);
    cola_salida.push({}); // Desbloquear la cola
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

} // namespace servidor