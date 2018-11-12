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
     */
    Terreno(const char *ruta_csv);

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
    std::vector<int> obtener_vecinos(int nodo) const override;

    /**
     * \brief Busca un camino mínimo entre inicio y fin.
     * 
     * Devuelve un vector con cada posición a recorrer.
     */
    std::vector<Posicion> buscar_camino_minimo(const Posicion& inicio, 
        const Posicion& fin) const;
    
    bool rango_valido_edificio(int x_, int y_,std::pair<int,int>& dim);
    bool rango_valido_tropa(int x_, int y_,std::pair<int,int>& dim);
    bool tiene_edificio(int x_, int y_);
    void agregar_edificio(int x_, int y_,std::pair<int,int>& dim);
    void eliminar_edificio(Posicion& pos,std::pair<int,int>& dim);
    bool es_caminable(int x_, int y_);
    bool hay_tropa(int x_, int y_);
    void agregar_tropa(int x_, int y_,std::pair<int,int>& dim);
    void eliminar_tropa(Posicion& pos,std::pair<int,int>& dim);

private:
    /**
     * \brief Representación del terreno como un arreglo de celdas.
     * 
     * El terreno se representa como un arreglo de celdas de 32x32 píxeles.
     */
    std::vector<std::vector<Celda>> terreno;

    /**
     * \brief Dimensiones del terreno, en celdas.
     */
    int ancho = 0, alto = 0;

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
