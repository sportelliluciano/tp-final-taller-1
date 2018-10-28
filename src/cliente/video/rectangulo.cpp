#include "cliente/video/rectangulo.h"

namespace cliente {

Rectangulo::Rectangulo(int x, int y, int w, int h) {
    sdl_rect.x = x;
    sdl_rect.y = y;
    sdl_rect.w = w;
    sdl_rect.h = h;
}

} // namespace cliente
