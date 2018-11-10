#ifndef _GUSANO_ARENA_H_
#define _GUSANO_ARENA_H_

#include "cliente/modelo/sprite_animado.h"
#include "cliente/modelo/terreno.h"

namespace cliente {

/**
 * \brief Representación del gusano de arena
 */
class GusanoArena {
public:
    /**
     * \brief Constructor.
     */
    GusanoArena(Terreno& terreno);

    /**
     * \brief Hace aparecer al gusano de arena en la posición indicada.
     * 
     * Muestra el gusano de arena en la posición indicada. (x, y) es la 
     * posición del gusano en celdas.
     */
    void aparecer(int x_, int y_);

    /**
     * \brief Renderiza el gusano sobre la ventana.
     * 
     * Si el gusano no está en pantalla la función no hace nada.
     */
    void renderizar(Ventana& ventana);

private:
    Terreno& terreno;
    SpriteAnimado sprite;
    int x, y;
    bool mostrar = false;
};

} // namespace cliente

#endif // _GUSANO_ARENA_H_
