#include "comun/error_conexion.h"

#include <stdexcept>
#include <string>



ErrorConexion::ErrorConexion() 
    : std::runtime_error("Se perdio la sincronización") 
{ }

ErrorConexion::ErrorConexion(const std::string& mensaje)
    : std::runtime_error(mensaje)
{ }


