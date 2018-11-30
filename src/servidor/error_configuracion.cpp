#include "servidor/error_configuracion.h"

#include <stdexcept>

namespace servidor {

ErrorConfiguracion::ErrorConfiguracion(const std::string& mensaje)
: std::runtime_error(mensaje)
{ }

} // namespace servidor
