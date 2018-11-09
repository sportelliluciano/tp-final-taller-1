#include "error_socket.h"

#include <cerrno>
#include <cstring>

#include <sstream>
#include <string>
#include <exception>

ErrorSocket::ErrorSocket(const std::string& funcion) {
    errno_guardado = errno;
    mensaje_ex = "[" + funcion + "]: " + strerror(errno);
}

ErrorSocket::ErrorSocket(const std::string& funcion,
	const std::string& mensaje, int errcode) {
    errno_guardado = errcode;
    std::stringstream s;
    s << "[" << funcion << "]: " << mensaje;
    mensaje_ex = s.str();
}

const char* ErrorSocket::what() const throw() {
    return mensaje_ex.c_str();
}

int ErrorSocket::error_code() const {
    return errno_guardado;
}
