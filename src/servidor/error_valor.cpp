#include "common_error_valor.h"

#include <stdexcept>
#include <string>

ErrorValor::ErrorValor(const std::string& mensaje)
    : std::runtime_error(mensaje)
{ }
