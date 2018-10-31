#ifndef _CONEXION_H_
#define _CONEXION_H_

#include <cstdlib>

#include <string>
#include <vector>

#include "conexion/error_socket.h"
#include "conexion/error_conexion.h"
#include "conexion/evento.h"
#include "conexion/socket_conexion.h"

namespace conexion {

/**
 * Clase Conexión.
 * 
 * Permite enviar distintos tipos de datos a través de una conexión TCP.
 * 
 * Esta clase provee métodos para enviar y recibir distintos tipos de dato entre
 * el cliente y el servidor. En particular permite enviar datos de tipo
 * booleano, objeto (serializado), entero, string o comando.
 * 
 * El protocolo para transmisión es el siguiente:
 * Se envía un byte para indicar qué tipo de dato se está enviando y luego
 * se envían 0 o más bytes correspondientes al dato a enviar.
 * 
 * Cada tipo de dato indica su longitud según se considere necesario.
 * Detalle de tipos:
 * 
 * Booleano: Existen dos tipos de dato (verdadero o falso), no necesita bytes
 *  extra.
 * Entero: Se envía el tipo entero + 4 bytes almacenados como big-endian.
 * Objeto: Se envía el tipo objeto, seguido de un entero indicando la longitud
 *  de la serializacion y luego los bytes serializados.
 * String: Se envía el tipo string + los bytes del string, para indicar la
 *  finalización del string se envía un byte 0.
 * Comando: Se envía el tipo comando y un byte identificando el comando.
 */

class Conexion {
public:
    /**
     * Constructor por defecto.
     */
    Conexion();

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
     * recibir_x: Recibe un dato desde la conexión. Devuelve true si la 
     * recepción fue correcta.
     * 
     * Si el método devuelve false, entonces el servidor envío un tipo de dato
     * que no es el que se esperaba recibir.
     * 
     * Puede lanzar ErrorSocket si la conexion se cierra antes de recibir
     * o se produce algún otro error en la misma.
     */

    Evento* recibir_evento();
    
    /**
     * enviar_x: Envía un dato a través de la conexión.
     * 
     * Puede lanzar ErrorSocket si la conexión se cierra o se produce algún
     * otro error en la misma durante el envío.
     */

    void enviar_evento(const Evento& evento);
    
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
    bool esta_conectado_rd = true, esta_conectado_wr = true;

    /* Deshabilitar copia */
    Conexion(const Conexion& otro) = delete;
    Conexion& operator=(const Conexion& otro) = delete;

    /**
     * Espera exactamente cantidad bytes desde la conexión.
     * La función bloquea hasta que se haya recibido la cantidad especificada.
     * 
     * Puede devolver menos que cantidad en caso de que la conexión se cierre
     * antes de que se reciba la cantidad esperada.
     * 
     * Si se produce algún error durante la recepción se lanzará ErrorSocket.
     */
    size_t recibir_bytes(uint8_t *buffer_, size_t cantidad);

    /**
     * Envía los bytes a través de la conexión. 
     * La función bloquea hasta que todos los bytes hayan sido enviados.
     * 
     * Devuelve la cantidad de bytes enviados, que puede ser menor a cantidad
     * en el caso en que la conexión se cierre antes de terminar el envío.
     * 
     * Si se produce algún error durante el envío lanzará ErrorSocket.
     */
    size_t enviar_bytes(const uint8_t *buffer_, size_t cantidad);

    /**
     * Lee un byte desde la conexión.
     */
    uint8_t leer_uint8();
};

} // namespace conexion

#endif // _CONEXION_H_ 
