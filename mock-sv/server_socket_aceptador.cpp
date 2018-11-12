#include "server_socket_aceptador.h"

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include <string>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../src/conexion/error_socket.h"
#include "../src/conexion/socket_conexion.h"

namespace conexion {

SocketAceptador::SocketAceptador(const std::string& servicio) 
    : socket_escucha(SOCKET_INVALIDO), detencion_solicitada(false) {
    struct addrinfo hints;
    struct addrinfo *ptr = NULL;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int err = getaddrinfo(NULL, servicio.c_str(), &hints, &ptr);
    if (err == SOCKET_ERROR)
        throw ErrorSocket("getaddrinfo", gai_strerror(err), err);

    socket_escucha = socket(ptr->ai_family, ptr->ai_socktype, 
        ptr->ai_protocol);

    if (socket_escucha == SOCKET_INVALIDO) {
        freeaddrinfo(ptr);
        throw ErrorSocket("socket");
    }

    err = bind(socket_escucha, ptr->ai_addr, ptr->ai_addrlen);
    if (err == SOCKET_ERROR) {
        close(socket_escucha);
        freeaddrinfo(ptr);
        throw ErrorSocket("bind");
    }
    freeaddrinfo(ptr);

    if (listen(socket_escucha, 20) == SOCKET_ERROR) {
        close(socket_escucha);
        throw ErrorSocket("listen");
    }
}

bool SocketAceptador::esperar_conexion(SocketConexion& conexion_recibida) {
    int s = accept(socket_escucha, NULL, NULL);
    if (s == SOCKET_INVALIDO) {
        if (detencion_solicitada)
            return false;
        
        throw ErrorSocket("accept");
    }

    conexion_recibida = SocketConexion(s);
    return true;
}

void SocketAceptador::detener() noexcept {
    detencion_solicitada = true;
    cerrar();
}

SocketAceptador::~SocketAceptador() noexcept {
    detener();
}

void SocketAceptador::cerrar() noexcept {
    if (socket_escucha != SOCKET_INVALIDO) {
        shutdown(socket_escucha, SHUT_RDWR);
        close(socket_escucha);
        socket_escucha = SOCKET_INVALIDO;
    }
}

} // namespace conexion
