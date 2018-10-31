#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "libs/json.hpp"

#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Sprite.
 */
class Sprite {
public:
    Sprite() = default;

    Sprite(const nlohmann::json& sprite);

    Sprite(int id, int desplazamiento_x, int desplazamiento_y);

    void desplazar(int desplazamiento_x, int desplazamiento_y);

    void renderizar(Ventana& ventana, int x, int y);

private:
    int id = -1;
    int dx = 0, dy = 0;
};

} // namespace cliente

#endif // _SPRITE_H_
