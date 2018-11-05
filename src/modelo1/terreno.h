#ifndef _TERRENO_H_
#define _TERRENO_H_

#include <vector>

#include "modelo/celda.h"
#include "modelo/grafo.h"
#include "modelo/posicion.h"

namespace modelo {

/**
 * \brief Terreno de juego.
 * 
 * El terreno de juego se divide en celdas que pueden tener distintos tipos:
 * Arena: Todas las unidades pueden moverse a través de este tipo.
 * Dunas: Todas las unidades pueden moverse a través de este tipo pero a la
 *  mitad de la velocidad.
 * Roca: Es el único tipo lo suficientemente sólido para construir sobre él,
 *  todas las unidades pueden moverse sobre este terreno.
 * Cimas: Se encuentra sobre arena o roca, sólo las unidades de infantería 
 *  pueden moverse a través de este tipo.
 * Precipicios: Ninguna unidad puede moverse a través de este tipo, funciona
 *  como barrera natural.
 */
class Terreno : public Grafo {
public:
    /**
     * \brief Constructor.
     * 
     * Crea un terreno de tamaño ancho x alto.
     * Tanto el alto como el ancho se miden en cantidad de celdas.
     */
    Terreno(int ancho, int alto);

    /**
     * \brief Devuelve una referencia a la celda en la posición (x, y).
     * 
     * Permite obtener una referencia constante a la celda en la posición 
     * (x, y). Si no existe dicha celda lanzará excepción (TODO).
     */
    Celda& obtener_celda(int x, int y);

    /**
     * \brief Devuelve el ancho del terreno.
     */
    int obtener_ancho() const;

    /**
     * \brief Devuelve el alto del terreno.
     */
    int obtener_alto() const;

    /**
     * \brief Interfaz Grafo: Devuelve los vecinos a un nodo.
     */
    std::vector<int> obtener_vecinos(int nodo) const;

    /**
     * \brief Busca un camino mínimo entre inicio y fin.
     * 
     * Devuelve un vector con cada posición a recorrer.
     */
    std::vector<Posicion> buscar_camino_minimo(const Celda& inicio, 
        const Celda& fin) const;

private:
    std::vector<std::vector<Celda>> terreno;
    int ancho_, alto_;

    /**
     * \brief Obtiene un hash reversible a partir de una posición discreta.
     */
    int hash_posicion(int x, int y) const;

    /**
     * \brief Revierte un hash a una posición discreta.
     */
    void revertir_hash(int hash, int& x, int& y) const;
};

} // namespace modelo

#endif // _TERRENO_H_
