#ifndef _SOCKET_ACEPTADOR_H_
#define _SOCKET_ACEPTADOR_H_

#include <string>

#include "error_socket.h"
#include "socket_conexion.h"

/**
 * Encapsulamiento RAII para un socket que acepta conexiones entrantes.
 */
class SocketAceptador {
public:
    /**
     * Inicia un socket aceptador escuchando en el servicio indicado.
     * Lanza ErrorSocket en caso de que se produzca un error.
     */
    explicit SocketAceptador(const std::string& servicio);

    /**
     * Espera a que un nuevo cliente se conecte; la conexión recibida se 
     * asigna a conexion_recibida.
     * El método bloquea hasta que una conexión se acepte o se detenga el
     * aceptador.
     * 
     * Devuelve true si se aceptó una nueva conexión o false en caso de que se
     * haya detenido el aceptador mediante el método detener.
     * 
     * Lanza ErrorSocket si se produce algún error al aceptar.
     */
    bool esperar_conexion(SocketConexion& conexion_recibida);

    /**
     * Cierra el socket aceptador e indica que la detención fue solicitada.
     * 
     * Esto provoca que cualquier llamado bloqueado en esperar_conexion termine
     * inmediatamente devolviendo false.
     * 
     * Se garantiza que este método no lanzará ninguna excepción.
     */
    void detener() noexcept;

    /**
     * Destructor. Cierra y libera el socket aceptador.
     */
    ~SocketAceptador() noexcept;

private:
    /* File descriptor para el socket aceptador. */
    int socket_escucha = SOCKET_INVALIDO;

    /* Flag para indicar si debe lanzar una excepción un error en accept. */
    bool detencion_solicitada = false;

    /**
     * Cierra el socket aceptador liberando sus recursos.
     * 
     * Se garantiza que este método no lanzará excepciones.
     * 
     * Llamar a este método sobre un socket aceptador cerrado no tiene ningún
     * efecto.
     */
    void cerrar() noexcept;
};

#endif // _SOCKET_ACEPTADOR_H_
