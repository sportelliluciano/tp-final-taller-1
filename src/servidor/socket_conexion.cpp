#include "socket_conexion.h"

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

SocketConexion::SocketConexion() : socket_conexion(SOCKET_INVALIDO) { }

SocketConexion::SocketConexion(int fd) : socket_conexion(fd) {
    if (fd == SOCKET_INVALIDO)
        throw ErrorSocket("SocketConexion(int)", "Socket inválido", EBADF);
}

SocketConexion::SocketConexion(const std::string& direccion, 
    const std::string& servicio) {
    struct addrinfo hints;
    struct addrinfo *result = NULL, *ptr = NULL;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;

    int err = getaddrinfo(direccion.c_str(), servicio.c_str(), 
        &hints, &result);
    
    if (err != 0)
        throw ErrorSocket("getaddrinfo", gai_strerror(err), err);

    bool esta_conectado = false;

    for (ptr = result; ptr != NULL && !esta_conectado; ptr = ptr->ai_next) {
        socket_conexion = 
            socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        
        if (socket_conexion == SOCKET_INVALIDO)
            continue;
        
        err = connect(socket_conexion, ptr->ai_addr, ptr->ai_addrlen);
        
        if (err == SOCKET_ERROR)
            close(socket_conexion);

        esta_conectado = (err != SOCKET_ERROR);
    }
    freeaddrinfo(result);
    
    if (!esta_conectado) {
        throw ErrorSocket("connect", 
            "No se pudo conectar al destino", EINVAL);
    }
}

SocketConexion::SocketConexion(SocketConexion&& otro) {
    socket_conexion = otro.socket_conexion;
    otro.socket_conexion = SOCKET_INVALIDO;
}

SocketConexion& SocketConexion::operator=(SocketConexion&& otro) {
    if (this == &otro)
        return *this;
    
    if (socket_conexion != SOCKET_INVALIDO) {
        // Liberar recursos de una posible conexión previa.
        shutdown(true, true);
        close();
    }

    socket_conexion = otro.socket_conexion;
    otro.socket_conexion = SOCKET_INVALIDO;
    return *this;
}

size_t SocketConexion::recv(uint8_t *buffer, size_t largo) {
    ssize_t bytes_recibidos = ::recv(socket_conexion, buffer, largo, 0);
    
    if (bytes_recibidos == SOCKET_ERROR)
        throw ErrorSocket("recv");
    
    return bytes_recibidos;
}

size_t SocketConexion::send(const uint8_t *buffer, size_t largo) {
    ssize_t bytes_enviados = ::send(socket_conexion, buffer, largo, 
        MSG_NOSIGNAL);
    
    if (bytes_enviados == SOCKET_ERROR)
        throw ErrorSocket("send");

    return bytes_enviados;
}

void SocketConexion::shutdown(bool shut_rd, bool shut_wr, bool ignorar_error) {
    int how = SHUT_RDWR;
    if (shut_rd && !shut_wr)
        how = SHUT_RD;
    else if (!shut_rd && shut_wr)
        how = SHUT_WR;
    else if (!shut_rd && !shut_wr)
        return;
    
    if ((::shutdown(socket_conexion, how) == SOCKET_ERROR) && !ignorar_error)
        throw ErrorSocket("shutdown");
}

void SocketConexion::close(bool ignorar_error) {
    int retval = ::close(socket_conexion);
    socket_conexion = SOCKET_INVALIDO;

    if ((retval == SOCKET_ERROR) && !ignorar_error)
        throw ErrorSocket("close");
}

SocketConexion::~SocketConexion() {
    if (socket_conexion != SOCKET_INVALIDO) {
        // Prevenir lanzamiento de excepciones en el destructor.
        shutdown(true, true, true);
        close(true);
    }
}
