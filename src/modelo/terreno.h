#ifndef _TERRENO_H_
#define _TERRENO_H_

#include <vector>
#include <unordered_map>
#include <utility>

#include "libs/json.hpp"

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
     */
    Terreno();

    /**
     * \brief Inicializa el terreno a partir de un mapa en formato JSON.
     */
    void inicializar(const nlohmann::json& mapa);

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
    /**
     * \brief Determina si se puede construir un edificio en la posición
     *        indicada.
     * 
     * Un edificio se puede construir si:
     * - Las celdas donde se va a construir son de roca
     * - Las celdas donde se va a construir están libres
     * - Hay un edificio del jugador a 5 casillas o menos
     * 
     * Si alguna de las condiciones no se cumple entonces el edificio no se 
     * podrá construir.
     */
    bool puede_construir_edificio(int x, int y, std::pair<int,int>& dim);
    
    bool rango_valido_tropa(int x, int y, std::pair<int,int>& dim);
    
    /**
     * \brief Marca la celda en la posición (x, y) indicando que hay un
     *        edificio en la misma.
     */
    void agregar_edificio(int x, int y, std::pair<int,int>& dim);

    /**
     * \brief Elimina la marca la celda en la posición (x, y) indicando que 
     *        hay un edificio en la misma.
     */
    void eliminar_edificio(Posicion& pos, std::pair<int,int>& dim);
    
    /**
     * \brief Marca la celda en la posición pasada por parámetro indicando que
     *        hay una tropa en la misma.
     */
    void agregar_tropa(const Posicion& posicion, std::pair<int,int>& dim);

    /**
     * \brief Elimina la marca de la celda en la posición pasada por parámetro 
     *        indicando que hay una tropa en la misma.
     */
    void eliminar_tropa(const Posicion& posicion, std::pair<int,int>& dim);

    /**
     * \brief Obtiene la primer posición que se puede caminar más cercana a 
     *        posicion_inicial.
     * 
     * Si la posicion inicial es caminable se devolverá la misma posición.
     */
    Posicion obtener_posicion_caminable_cercana(
        const Posicion& posicion_inicial);

    /**
     * \brief Agrega un indicador de refinería en la posición (x, y) para el
     *        jugador con id = id_jugador.
     */
    void agregar_refineria(int x_, int y_,int id_jugador);

    void eliminar_refineria(const Posicion& posicion,int id_jugador);

    void agregar_centro(int x_, int y_,int id_jugador);

    int eliminar_centro(int id_jugador);

    Posicion& obtener_centro_posicion(int id_jugador);

    /**
     * \brief Obtiene las posiciones de las refinerías del jugador.
     */
    
    Posicion obtener_refinerias_cercana(const Posicion& pos,int id_jugador);

    bool es_especia(const Posicion& posicion);
    
    Posicion obtener_especia_cercana(Posicion& posicion_i);

private:
    /**
     * \brief Representación del terreno como un arreglo de celdas.
     * 
     * El terreno se representa como un arreglo de celdas de 32x32 píxeles.
     */
    std::vector<std::vector<Celda>> terreno;

    std::unordered_map<int,std::vector<Posicion>> refinerias;

    std::unordered_map<int,Posicion> centros;

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

    /**
     * \brief Devuelve true si se puede caminar por la celda (x, y).
     */
    bool es_caminable(int x, int y);

    /**
     * \brief Devuelve true si hay un edificio en la celda (x, y).
     */
    bool tiene_edificio(int x, int y);

    /**
     * \brief Devuelve true si hay una tropa en la celda (x, y).
     */
    bool hay_tropa(int x, int y);
};

} // namespace modelo

#endif // _TERRENO_H_
