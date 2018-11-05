#ifndef _ERROR_VALOR_H_
#define _ERROR_VALOR_H_

#include <stdexcept>
#include <string>

/**
 * Representa un error en el valor de una entidad.
 * 
 * Esto puede significar que un archivo se pudo abrir y leer, pero su contenido
 * era incorrecto; o que una variable tiene el tipo correcto pero su valor no
 * se puede utilizar en un contexto determinado.
 */
class ErrorValor : public std::runtime_error {
public:
    explicit ErrorValor(const std::string& mensaje);
};

#endif // _ERROR_VALOR_H_
