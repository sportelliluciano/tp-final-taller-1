#include "a_estrella.h"

#include <limits>
#include <set>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include <iostream>

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
/*Herramienta de testeo
    void mostrar_set(std::vector<Nodo>* myset){
    std::cout << "myset contains:";
    for (std::vector<Nodo>::iterator it=(*myset).begin(); it!=(*myset).end(); ++it)
        std::cout << ' ' << (*it).id ;

    std::cout << '\n';

    }*/
    Nodo* esta(std::vector<Nodo>* myset,int id_vecino){
        for (auto it=(*myset).begin(); it!=(*myset).end(); ++it){
            if ((*it).id == id_vecino){
                return &(*it);
            }    
        }
        return nullptr;
    }
    Nodo mas_chico(std::vector<Nodo>* myset){
        Nodo tmp;
        double mas_chico = std::numeric_limits<double>::infinity();
        for (auto it=(*myset).begin(); it!=(*myset).end(); ++it){
            if ((*it).f_score < mas_chico){
                mas_chico = (*it).f_score;
            }
        }
        for (auto it=(*myset).begin(); it!=(*myset).end(); ++it){
            if ((*it).f_score == mas_chico){
                tmp = *it;
                myset->erase(it);
                return tmp;
            }    
        }
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
    std::vector<Nodo> nodos_sin_visitar;
    std::unordered_set<int> nodos_visitados;
    std::unordered_map<int, int> de_donde_vine;
    Nodo nodo(id_origen);
    nodo.g_score = 0;
    nodo.f_score = distancia_entre(id_origen, id_destino);
    nodos_sin_visitar.push_back(nodo);

    while (!nodos_sin_visitar.empty()) {
        const Nodo& actual = mas_chico(&nodos_sin_visitar);
        if (actual.id == id_destino){
            return reconstruir_camino(de_donde_vine, actual.id);
        }
        nodos_visitados.insert(actual.id);
        std::vector<int> vecinos = grafo->obtener_vecinos(actual.id);
        for (auto id_vecino=vecinos.begin(); id_vecino != vecinos.end(); 
            ++id_vecino) {
            if (nodos_visitados.count(*id_vecino) != 0){
                continue;
            }
            double g_score_tentativo = actual.g_score + 
                distancia_entre(*id_vecino, actual.id);    
            Nodo* vecino = esta(&nodos_sin_visitar, *id_vecino);
            if (vecino) {
                if (g_score_tentativo >= (*vecino).g_score){
                    continue;
                }
                de_donde_vine[(*vecino).id] = actual.id;
                (*vecino).g_score = g_score_tentativo;
                (*vecino).f_score = g_score_tentativo + 
                                    distancia_entre((*vecino).id, id_destino);
            } else {
                Nodo nuevo_nodo(*id_vecino);
                de_donde_vine[nuevo_nodo.id] = actual.id;
                nuevo_nodo.g_score = g_score_tentativo;
                nuevo_nodo.f_score = g_score_tentativo + 
                                distancia_entre(nuevo_nodo.id, id_destino);
                nodos_sin_visitar.push_back(nuevo_nodo);
            } 
        }
    }
    
    /** ¡No hay camino disponible! **/
    throw std::runtime_error("No hay ningún camino al destino.");
}

} // namespace modelo
