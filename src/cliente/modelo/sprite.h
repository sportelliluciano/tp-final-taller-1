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

    static Sprite desde_json(const nlohmann::json& data);

    explicit Sprite(const std::string& ruta_);

    explicit Sprite(int id, int desplazamiento_x = 0, int desplazamiento_y = 0);

    virtual void desplazar(int desplazamiento_x, int desplazamiento_y);

    virtual void renderizar(Ventana& ventana, int x, int y);

    virtual const Textura& obtener_textura(Ventana& ventana);

    virtual int obtener_alto(Ventana& ventana) const;
    virtual int obtener_ancho(Ventana& ventana) const;
    virtual int obtener_desplazamiento_x() const;
    virtual int obtener_desplazamiento_y() const;

    virtual int obtener_id() const;

private:
    int id = -1;
    int dx = 0, dy = 0;
    std::string ruta;
};

} // namespace cliente

#endif // _SPRITE_H_
