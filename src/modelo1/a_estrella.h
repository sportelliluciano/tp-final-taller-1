#ifndef _A_ESTRELLA_H_
#define _A_ESTRELLA_H_

#include <functional>
#include <unordered_map>

#include "modelo/grafo.h"

namespace modelo {

/**
 * \brief Implementación del algoritmo A*
 * 
 * Implementación del algoritmo A* utilizando como heurística la distancia
 * de línea de un punto al objetivo.
 */
class AEstrella {
public:
    /**
     * \brief Constructor.
     * 
     * funcion_distancia: Función auxiliar utilizada para el cálculo de 
     * distancia entre nodos, y en particular de la heurística usada por el 
     * algoritmo.
     * 
     * La firma de la función es la siguiente:
     * double funcion_distancia(int id_origen, int id_destino);
     * 
     * Donde id_origen e id_destino son los identificadores de los nodos de 
     * origen y destino.
     * 
     * Usualmente esta función calcula la distancia euclídea pero otras métricas
     * de distancia entre dos nodos podrían funcionar.
     */
    AEstrella(std::function<double(int, int)> funcion_distancia);

    /**
     * \brief Calcula un camino mínimo desde inicio a fin.
     * 
     * Devuelve el camino a seguir como un vector con los nodos que lo 
     * conforman. El primer elemento del vector es el nodo indicado por id_fin
     * mientras que el último elemento del vector es el nodo indicado por 
     * id_inicio.
     */
    std::vector<int> calcular_camino(const Grafo* grafo, int id_inicio, 
        int id_fin);

private:
    /**
     * \brief Heurística a utilizar durante el algoritmo. 
     * 
     * Calcula la distancia de línea entre dos nodos.
     */
    std::function<double(int, int)> distancia_entre;

    /**
     * \brief Reconstruye el camino encontrado por el algoritmo
     * 
     * mapa: Mapa de proveniencia de cada nodo. 
     * id_fin: ID del nodo final.
     */
    std::vector<int> reconstruir_camino(std::unordered_map<int, int> mapa, 
        int id_fin);
};

} // namespace modelo

#endif // _A_ESTRELLA_H_
