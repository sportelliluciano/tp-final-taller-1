#include "error_conexion.h"

#include <stdexcept>
#include <string>

ErrorConexion::ErrorConexion() 
    : std::runtime_error("Se perdio la sincronización") 
{ }

