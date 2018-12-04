#ifndef _CONEXION_H_
#define _CONEXION_H_

#include <memory>
#include <string>

#include "libs/json.hpp"

#include "comun/error_socket.h"
#include "comun/error_conexion.h"
#include "comun/socket_conexion.h"



/**
 * Clase Conexión.
 * 
 * Permite enviar y recibir datos en formato JSON a través de una conexión TCP.
 */

class Conexion {
public:
    /**
     * Constructor a partir de un socket conectado.
     */
    explicit Conexion(SocketConexion& socket);
    
    /**
     * Abre una conexión a un servidor.
     * 
     * direccion: IP o nombre de dominio del servidor.
     * servicio: puerto o nombre del servicio a conectar.
     * 
     * Lanza ErrorSocket si no puedo conectarse.
     */
    Conexion(const std::string& direccion, const std::string& servicio);

    /**
     * Constructor por movimiento 
     */
    Conexion(Conexion&& otro);

    /**
     * Asignación por movimiento 
     */
    Conexion& operator=(Conexion&& otro);

    /**
     * recibir_json: Recibe un dato desde la conexión. Devuelve true si la 
     * recepción fue correcta.
     * 
     * Si el método devuelve false, entonces el servidor envío un tipo de dato
     * que no es el que se esperaba recibir.
     * 
     * Puede lanzar ErrorSocket si la conexion se cierra antes de recibir
     * o se produce algún otro error en la misma.
     * 
     * Este método se puede ejecutar de manera asincrónica junto con 
     * enviar_json.
     */

    nlohmann::json recibir_json();
    
    /**
     * enviar_json: Envía un dato a través de la conexión.
     * 
     * Puede lanzar ErrorSocket si la conexión se cierra o se produce algún
     * otro error en la misma durante el envío.
     *
     * Este método se puede ejecutar de manera asincrónica junto con 
     * recibir_json.
     */

    void enviar_json(const nlohmann::json& data);
    
    /**
     * Devuelve true si ambos canales de la conexión están abiertos.
     * 
     * Una conexión _esta conectada_ si se puede leer *y* escribir 
     * a la misma.
     * 
     * Si el canal de lectura o el de escritura está cerrado se considera que 
     * la conexión no está conectada.
     */
    bool esta_conectada() const;

    /**
     * Cierra la conexión.
     * 
     * Este método puede ser llamado desde otro hilo para provocar el 
     * desbloqueo de una llamada a métodos que envíen o reciban datos.
     * 
     * En caso de que se produzca un error al cerrar la conexión se lanzará
     * ErrorSocket. Se puede evitar que el método lance una excepción seteando
     * el parámetro por defecto _ignorar_errores_ a true.
     * 
     * Llamar a este método sobre una conexión que ya fue cerrada no tiene
     * ningún efecto.
     */
    void cerrar(bool ignorar_errores = false);

    /**
     * Destructor por defecto 
     */
    ~Conexion();

private:
    SocketConexion conexion;
    std::unique_ptr<uint8_t[]> buffer_recepcion;

    /* Deshabilitar copia */
    Conexion(const Conexion& otro) = delete;
    Conexion& operator=(const Conexion& otro) = delete;
};



#endif // _CONEXION_H_ 
