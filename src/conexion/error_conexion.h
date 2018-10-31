#ifndef _ERROR_CONEXION_H_
#define _ERROR_CONEXION_H_

#include <stdexcept>
#include <string>

namespace conexion {

/**
 * Representa un error de protocolo.
 * 
 * Esta excepción es levantada cuando el cliente o el servidor envían un tipo
 * de dato inesperado.
 */
class ErrorConexion : public std::runtime_error {
public:
    ErrorConexion();
    ErrorConexion(const std::string& mensaje);
};

} // namespace conexion

#endif // _ERROR_CONEXION_H_

