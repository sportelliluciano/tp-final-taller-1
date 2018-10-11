#ifndef _SOCKET_CONEXION_H_
#define _SOCKET_CONEXION_H_

#include <cerrno>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <string>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "error_socket.h"

/* File descriptor para un socket inválido */
#define SOCKET_INVALIDO -1

/* Código de retorno de error de funciones de sockets. */
#define SOCKET_ERROR -1

/**
 * Encapsulamiento RAII para un socket de conexión.
 */
class SocketConexion {
public:
    /* Constructor por defecto. */
    SocketConexion();
    
    /**
     * Constructor a partir de un file descriptor de un socket abierto 
     * previamente.
     * 
     * Si el file descriptor es SOCKET_INVALIDO lanzará una excepción.
     */
    explicit SocketConexion(int fd);
    
    /**
     * Abre un nuevo socket de conexión a un servidor.
     * 
     * direccion: IP o nombre de dominio del servidor
     * servicio: puerto o nombre de servicio a conectar
     * 
     * Lanza ErrorSocket si no se puedo conectar al servidor o si se produjo
     * algún otro error.
     */
    SocketConexion(const std::string& direccion, const std::string& servicio);

    /**
     * Constructor por movimiento.
     */
    SocketConexion(SocketConexion&& otro);

    /**
     * Asignación por movimiento.
     * 
     * Si hay una conexión establecida, la libera y luego realiza el movimiento.
     * 
     * Si falla la liberación de recursos para la conexión actual se lanzará
     * ErrorSocket y la otra conexión no será modificada.
     */
    SocketConexion& operator=(SocketConexion&& otro);
    
    /**
     * Recibe como máximo _largo_ bytes desde la conexión.
     * 
     * Este método encapsula la función recv (man 2 recv).
     * 
     * Devuelve 0 si la conexión o el canal de lectura está cerrado.
     * Lanza ErrorSocket si se produce otro tipo de error.
     */
    size_t recv(uint8_t *buffer, size_t largo);

    /**
     * Envía hasta _largo_ bytes hacia la conexión.
     * Podría no enviar el buffer completo, en cuyo caso debería volver a
     * llamarse al método con los bytes restantes.
     * 
     * Este método encapsula la función send (man 2 send).
     * 
     * Devuelve 0 si la conexión o el canal de escritura está cerrado.
     * Lanza ErrorSocket si se produce algún error.
     */
    size_t send(const uint8_t *buffer, size_t largo);

    /**
     * Cierra el canal de escritura o lectura de la conexión.
     * 
     * Este método encapsula la función shutdown (man 2 shutdown).
     * 
     * Este método por defecto lanza ErrorSocket si se produce un problema al 
     * intentar cerrar el/los canal(es). 
     * Puede establecerse ignorar_error en true para silenciar esta excepción.
     */
    void shutdown(bool shut_rd, bool shut_wr, bool ignorar_error = false);

    /**
     * Cierra la conexión y libera los recursos asociados a la misma.
     * 
     * Este método encapsula la función close (man 2 close).
     * 
     * Este método por defecto lanza ErrorSocket si se produce un problema al 
     * intentar cerrar el/los canal(es). 
     * Puede establecerse ignorar_error en true para silenciar esta excepción.
     */
    void close(bool ignorar_error = false);

    /**
     * Libera el socket y los recursos asociados al mismo.
     */
    ~SocketConexion();

private:
    /* File descriptor para el socket. */
    int socket_conexion;

    /* Deshabilitar copia */
    SocketConexion(const SocketConexion& otro) = delete;
    SocketConexion& operator=(const SocketConexion& otro) = delete;
};

#endif // _SOCKET_CONEXION_H_
