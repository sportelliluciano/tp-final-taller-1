#ifndef _CLIENTE_H_
#define _CLIENTE_H_

#include <mutex>
#include <string>
#include <thread>

#include "libs/json.hpp"

#include "conexion/conexion.h"
#include "conexion/socket_conexion.h"
#include "servidor/cola_protegida.h"

namespace servidor {

/**
 * \brief Encapsula un cliente que puede enviar y recibir mensajes de forma
 *        asíncrona.
 */
class Cliente {
public:
    /**
     * \brief Crea un nuevo cliente a partir del socket indicado.
     */
    Cliente(conexion::SocketConexion socket_conexion);

    /**
     * \brief Constructor por movimiento
     */
    Cliente(Cliente&& otro);

    /**
     * \brief Inicia la comunicación asincrónica.
     * 
     * Cualquier dato enviado antes de haberse iniciado la comunicación
     * se enviará en este momento.
     */
    void iniciar_async();

    /**
     * \brief Detiene la comunicación asincrónica y cierra la conexión.
     */
    void detener_async();

    /**
     * \brief Envia los datos al cliente. 
     * 
     * Envía asincrónicamente datos al cliente. Esta función no bloquea. 
     * Los datos se enviarán asíncronamente cuando la conexión lo permita.
     */
    void enviar(const nlohmann::json& data);

    /**
     * \brief Callback a ejecutar al recibir un nuevo dato.
     * 
     * Este callback se ejecutará asíncronamente al recibir un nuevo dato.
     * El mismo se ejecutará en el mismo hilo de recepción, asegurando que no
     * llegará otro dato durante su ejecución.
     */
    void al_recibir_datos(
        std::function<void(const nlohmann::json& data)> callback);

    /**
     * \brief Devuelve true si la conexión con el cliente sigue abierta.
     * 
     * Si este método devuelve false debería verificarse si se produjo algún
     * error mediante el método hubo_error.
     */
    bool esta_conectado() const;

    /**
     * \brief Devuelve true si se produjo algún error en la conexión.
     */
    bool hubo_error() const;

    /**
     * \brief Obtiene el mensaje del último error ocurrido.
     * 
     * Si se produjeron errores en el envío y en la recepción sólo se devolverá
     * el mensaje de error correspondiente al envío.
     */
    const std::string& obtener_error() const;

private:
    conexion::Conexion conexion;
    std::thread hilo_emisor, hilo_receptor;
    ColaProtegida cola_salida;

    std::mutex m_cb_al_recibir_datos;
    std::function<void(const nlohmann::json&)> cb_al_recibir_datos = nullptr;

    bool detencion_solicitada = false;
    bool hubo_error_emisor = false, hubo_error_receptor = false;
    std::string error_emisor, error_receptor;

    bool async_iniciado = false;
};

} // namespace servidor

#endif // _CLIENTE_H_
