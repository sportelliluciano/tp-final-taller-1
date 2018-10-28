#ifndef _CELDA_H_
#define _CELDA_H_

#include <vector>

#include "cliente/modelo/tropa.h"

namespace cliente {

class Celda { 
public:
    Celda() = default;
    Celda(int tile_no, int x_, int y_);

    /**
     * \brief Devuelve la posición en la coordenada X.
     */
    int x() const;

    /**
     * \brief Devuelve la posición en la coordenada Y.
     */
    int y() const;

    /**
     * \brief Devuelve el número de mosaico a mostrar de la hoja de mosaicos
     * del juego.
     */
    int numero_mosaico() const;

    /**
     * \brief Devuelve true si hay al menos una tropa en la celda actual.
     */
    bool contiene_tropas() const;

    /**
     * \brief Devuelve las tropas que estén paradas sobre la celda actual.
     */
    std::vector<Tropa*> obtener_tropas() const;

private:
    int posicion_x, posicion_y;
    int tipo_celda;
    
    /**
     * Número de mosaico de la hoja de mosaicos del juego.
     */
    int numero_tile;
};

} // namespace cliente


#endif // _CELDA_H_
