#ifndef _ERROR_SDL_H_
#define _ERROR_SDL_H_

#include <exception>
#include <string>

/**
 * \brief Excepción en una función SDL.
 * 
 * Indica qué función de SDL falló y el mensaje de error correspondiente.
 */
class ErrorSDL : public std::exception {
public: 
    /**
     * \brief Falló una función SDL.
     * 
     * Indicar en el constructor qué función fue la que falló, el mensaje de
     * error se obtiene automáticamente mediante SDL_GetError.
     */
    explicit ErrorSDL(const std::string& funcion_sdl);

    /**
     * \brief Obtiene el mensaje de error de la excepción.
     * 
     * Sobrecarga del método what.
     */
    const char *what() const throw() override;

private:
    std::string mensaje;
};

#endif // _ERROR_SDL_H_
