#include "conexion/error_conexion.h"

#include <stdexcept>
#include <string>

namespace conexion {

ErrorConexion::ErrorConexion() 
    : std::runtime_error("Se perdio la sincronización") 
{ }

ErrorConexion::ErrorConexion(const std::string& mensaje)
    : std::runtime_error(mensaje)
{ }

} // namespace conexion
