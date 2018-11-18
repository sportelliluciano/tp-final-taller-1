#ifndef _SPRITE_COMPUESTO_H_
#define _SPRITE_COMPUESTO_H_

#include <sstream>
#include <string>
#include <vector>

#include "cliente/modelo/sprite.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Sprite estático compuesto por otros sprites.
 */
class SpriteCompuesto : public Sprite {
public:
    /**
     * \brief Crea un nuevo sprite animado vacío.
     * 
     * Llamar a renderizar sobre este sprite no tienen ningún efecto.
     */
    SpriteCompuesto() = default;

    SpriteCompuesto(const SpriteCompuesto& otro);

    SpriteCompuesto& operator=(const SpriteCompuesto& otro);


    // SpriteCompuesto(const nlohmann::json& sprite);

    /**
     * \brief Crea un nuevo sprite compuesto a partir de los sprites indicados.
     * 
     * Los sprites se renderizarán en el orden en que se encuentran en el 
     * arreglo. El resultado de utilizar este constructor es el mismo que 
     * utilizar el constructor por defecto y luego agregar los cuadros uno por
     * uno en el orden en que se encuentran.
     */
    SpriteCompuesto(const std::vector<int>& sprites_);
    SpriteCompuesto(const std::vector<Sprite>& sprites_);

    /**
     * \brief Agrega un nuevo sprite estático al sprite compuesto.
     * 
     * Agrega un nuevo sprite estático al sprite compuesto. 
     * (dx, dy) es el desplazamiento en píxeles desde la posición de dibujado.
     * 
     * Los sprites se renderizan en el orden en el que fueron agregados, lo que
     * significa que si se agrega un sprite A y luego un sprite B, el sprite
     * B quedará por encima de A.
     */
    void agregar_sprite(const Sprite& sprite);
    void agregar_sprite(int id_sprite, int despl_x = 0, int despl_y = 0);

    /**
     * \brief Modifica el desplazamiento del sprite en la posición id.
     * 
     * Modifica el desplazamiento del sprite en la posición id, donde id es 
     * el número de orden en que se agregó el sprite, siendo 0 el primer sprite.
     * 
     * Lanza runtime_error si el cuadro no existe.
     */
    void desplazar_subsprite(int id, int despl_x, int despl_y);

    /**
     * \brief Renderiza el sprite en la posición (x, y).
     */
    void renderizar(Ventana& ventana, int x, int y);

    /**
     * \brief Obtiene la textura del sprite.
     */
    const Textura& obtener_textura(Ventana& ventana) override;

private:
    std::vector<Sprite> sprites;
    std::stringstream s_nombre;
    bool invalidar = true;
};

} // namespace cliente

#endif // _SPRITE_COMPUESTO_H_
