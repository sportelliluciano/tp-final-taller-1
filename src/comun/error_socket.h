#ifndef _ERROR_SOCKET_H_
#define _ERROR_SOCKET_H_

#include <string>
#include <exception>

/**
 * Representa un error en un llamado a una función de sockets del 
 * sistema operativo.
 * 
 * Esta función encapsula el código de error devuelto por la función o 
 * almacenado en errno, el nombre de la función que falló y eventualmente
 * un mensaje personalizado de ser necesario.
 */
class ErrorSocket : public std::exception {
public:
    /**
     * Constructor para errores en funciones que alteran errno.
     * 
     * Este constructor recibe únicamente el nombre de la función que falló
     * y almacena internamente el valor de errno y su mensaje de error provisto
     * por el sistema operativo.
     */
    explicit ErrorSocket(const std::string& funcion);

    /**
     * Constructor para errores en funciones que devuelven un código de
     * error.
     * 
     * Este constructor recibe el nombre de una función, el mensaje 
     * correspondiente al error y el código de error devuelto por la misma.
     */
    ErrorSocket(const std::string& funcion, const std::string& mensaje, 
        int errcode);

    /**
     * Método what. Devuelve un mensaje de error detallando que problema 
     * ocurrió.
     */
    const char* what() const throw();

    /**
     * Devuelve el código de error provisto en el constructor o encontrado en
     * errno en el momento de la construcción.
     */
    int error_code() const;

private:
    std::string mensaje_ex;
    int errno_guardado;
};

#endif // _ERROR_SOCKET_H_
