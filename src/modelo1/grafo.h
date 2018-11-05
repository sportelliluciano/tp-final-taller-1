#ifndef _GRAFO_H_
#define _GRAFO_H_

#include <vector>

namespace modelo {

/**
 * \brief Interfaz representando un grafo no dirigido. 
 * 
 * Esta interfaz se define para utilizar con algoritmos de búsqueda de caminos
 * que funcionan sobre un grafo.
 */
class Grafo {
public:
    /**
     * \brief Devuelve un vector con los vecinos al nodo indicado.
     * 
     * Esta función determina los vecinos del nodo indicado y devuelve un
     * vector con los ID de los nodos vecinos al mismo.
     */
    virtual std::vector<int> obtener_vecinos(int nodo) const = 0;
    virtual ~Grafo() { }
};

} // namespace modelo

#endif // _GRAFO_H_
