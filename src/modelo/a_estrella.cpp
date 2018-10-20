#include "modelo/a_estrella.h"

#include <limits>
#include <set>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace modelo {

AEstrella::AEstrella(std::function<double(int, int)> funcion_distancia) {
    distancia_entre = funcion_distancia;
}

std::vector<int> AEstrella::reconstruir_camino(
    std::unordered_map<int, int> de_donde_vine, int actual) {
    std::vector<int> resultado;
    
    while (de_donde_vine.count(actual) != 0) {
        resultado.push_back(actual);
        actual = de_donde_vine[actual];
    }
    
    return resultado;
}

std::vector<int> 
    AEstrella::calcular_camino(const Grafo* grafo, int id_origen, 
    int id_destino) {
    /**
     * \brief Nodo de A-Estrella.
     * 
     * Mantiene información sobre los nodos que el algoritmo está evaluando.
     * Admite una relación de orden donde un Nodo A es menor que un Nodo B si
     * A.f_score < B.f_score.
     */
    struct Nodo {
        int id; /* ID del nodo real */
        double g_score, f_score; /* Puntuaciones calculadas por el algoritmo. */

        Nodo() { 
            f_score = g_score = std::numeric_limits<double>::infinity();
        }

        explicit Nodo(int id_) : id(id_) {
            f_score = g_score = std::numeric_limits<double>::infinity();
        }

        bool operator<(const Nodo& otro) const { 
            return f_score < otro.f_score;
        }
    };

    std::unordered_map<int, Nodo> nodos; // Única instancia de los nodos.
    std::set<Nodo*> nodos_sin_visitar;
    std::unordered_set<int> nodos_visitados;
    std::unordered_map<int, int> de_donde_vine;
    

    nodos[id_origen] = Nodo(id_origen);
    nodos[id_origen].g_score = 0;
    nodos[id_origen].f_score = distancia_entre(id_origen, id_destino);
    nodos_sin_visitar.insert(&nodos[id_origen]);

    while (!nodos_sin_visitar.empty()) {
        const Nodo& actual = **nodos_sin_visitar.begin();
        if (actual.id == id_destino)
            return reconstruir_camino(de_donde_vine, actual.id);
        
        nodos_sin_visitar.erase(nodos_sin_visitar.begin());
        nodos_visitados.insert(actual.id);

        std::vector<int> vecinos = grafo->obtener_vecinos(actual.id);
        for (auto it=vecinos.begin(); it != vecinos.end(); ++it) {
            if (nodos.count(*it) == 0)
                nodos[*it] = Nodo(*it);

            Nodo& vecino = nodos[*it];
            
            if (nodos_visitados.count(vecino.id) != 0)
                continue;
            
            double g_score_tentativo = actual.g_score + 
                distancia_entre(vecino.id, actual.id);
                
            if (nodos_sin_visitar.count(&vecino) != 0) {
                if (g_score_tentativo >= vecino.g_score)
                    continue;
                else
                    nodos_sin_visitar.erase(&vecino);
            }

            de_donde_vine[vecino.id] = actual.id;
            vecino.g_score = g_score_tentativo;
            vecino.f_score = g_score_tentativo + 
                distancia_entre(vecino.id, id_destino);
            nodos_sin_visitar.insert(&vecino);
        }
    }
    
    /** ¡No hay camino disponible! **/
    throw std::runtime_error("No hay ningún camino al destino.");
}

} // namespace modelo
