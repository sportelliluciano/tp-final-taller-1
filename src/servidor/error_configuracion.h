#ifndef _ERROR_CONFIGURACION_H_
#define _ERROR_CONFIGURACION_H_

#include <stdexcept>

namespace servidor {

class ErrorConfiguracion : public std::runtime_error {
public:
    ErrorConfiguracion(const std::string& mensaje);
};

} // namespace servidor

#endif // _ERROR_CONFIGURACION_H_
