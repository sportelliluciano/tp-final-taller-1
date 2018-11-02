#ifndef _EDIFICIO_H_
#define _EDIFICIO_H_

#include <unordered_map>

#include "modelo/terreno.h"
#include "modelo/celda.h"

/**
 * \brief 
 * 
 *
 *
 */
namespace modelo {

/**
 * \brief 
 */
class Edificio{
    private:
    Terreno& terreno;
    Celda posicion;
    unsigned int energia;
    unsigned int costo;
    unsigned int dimensiones;
    unsigned int puntos_de_estructura;
    char tipo;

    public:
    Edificio(int x, int y, Terreno& terreno, unsigned int energia,
                                             unsigned int costo,
                                             unsigned int dimensiones,
                                             unsigned int puntos_de_estructura,
                                             char tipo);
    void recibir_dano(unsigned int dano);
    void Destruir();
};
}
#endif
