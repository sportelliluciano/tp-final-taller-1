#include "cliente/video/error_sdl.h"

#include <string>

#include <SDL2/SDL.h>

ErrorSDL::ErrorSDL(const std::string& funcion_sdl) {
    mensaje = funcion_sdl + ": " + SDL_GetError();
}

const char *ErrorSDL::what() const throw() {
    return mensaje.c_str();
}
