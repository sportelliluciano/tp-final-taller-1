#include "cliente/video/color.h"

#include <SDL2/SDL.h>

#include <sstream>
#include <string>

namespace cliente {

Color::Color() : Color(0, 0, 0, 0) { }

Color::Color(const SDL_Color* color_sdl) {
    color = *color_sdl;
}

Color::Color(int r, int g, int b, int a) {
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
}

std::string Color::str() const {
    std::stringstream s_color;
    s_color << "rgba(" 
            << color.r << ","
            << color.g << ","
            << color.b << ","
            << color.a << ")";
    
    return s_color.str();
}

const SDL_Color* Color::obtener_color() const {
    return &color;
}

} // namespace cliente