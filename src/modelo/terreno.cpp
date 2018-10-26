#include "modelo/terreno.h"

#include <cmath>
#include <iostream>

#include "modelo/a_estrella.h"
#include "modelo/celda.h"

namespace modelo {

Terreno::Terreno(int ancho, int alto) 
    : terreno(alto), ancho_(ancho), alto_(alto)
{
    for (int i=0;i<alto;i++) {
        terreno[i] = std::vector<Celda>(ancho);
        for (int j = 0; j < ancho; j++) {
            terreno[i][j] = Celda(i, j);
        }
    }
}

Celda& Terreno::obtener_celda(int x, int y) {
    return terreno[x][y];
}

int Terreno::obtener_ancho() const {
    return ancho_;
}

int Terreno::obtener_alto() const {
    return alto_;
}

int Terreno::hash_posicion(int x, int y) const {
    // Mientras esto no de overflow el hash no colisiona.
    // TODO: Mejorar esto?
    return (y * ancho_) + x;
}

void Terreno::revertir_hash(int hash, int& x, int& y) const {
    x = hash % ancho_;
    y = hash / ancho_;
}

// Esta función es más compleja de lo que parece.
// Devuelve los vecinos a la celda actual teniendo en cuenta que:
// - Sólo se puede ir en diagonal si las dos diagonales opuestas no están 
//   bloqueadas
std::vector<int> Terreno::obtener_vecinos(int nodo) const {
    int x, y;
    revertir_hash(nodo, x, y);
    
    std::vector<int> vecinos;

    // TODO: limpiar esto

    // Celdas que siempre son caminables (a menos que sean obstáculo)
    bool arriba = ((y - 1) > 0) && terreno[x][y - 1].es_caminable(), 
         abajo  = ((y + 1) < alto_) && terreno[x][y + 1].es_caminable(), 
         izquierda = ((x - 1) > 0) && terreno[x - 1][y].es_caminable(), 
         derecha = ((x + 1) < ancho_) && terreno[x + 1][y].es_caminable();
        
    // Celdas que son caminables sólo si las aledañas son caminables
    //  (y no son obstáculos)
    bool arriba_izq = (arriba && izquierda) && terreno[x-1][y-1].es_caminable(),
         arriba_der = (arriba && derecha) && terreno[x+1][y-1].es_caminable(),
         abajo_izq = (abajo && izquierda) && terreno[x-1][y+1].es_caminable(),
         abajo_der = (abajo && derecha) && terreno[x+1][y+1].es_caminable();

    if (arriba)
        vecinos.push_back(hash_posicion(x, y - 1));
    if (abajo)
        vecinos.push_back(hash_posicion(x, y + 1));
    if (izquierda)
        vecinos.push_back(hash_posicion(x - 1, y));
    if (derecha)
        vecinos.push_back(hash_posicion(x + 1, y));

    if (arriba_izq)
        vecinos.push_back(hash_posicion(x - 1, y - 1));
    if (arriba_der)
        vecinos.push_back(hash_posicion(x + 1, y - 1));
    if (abajo_izq)
        vecinos.push_back(hash_posicion(x - 1, y + 1));
    if (abajo_der)
        vecinos.push_back(hash_posicion(x + 1, y + 1));

    return vecinos;
}

std::vector<Posicion> 
    Terreno::buscar_camino_minimo(const Celda& inicio, const Celda& fin) const {
    struct CalcularDistancia {
        CalcularDistancia(const Terreno& terreno_) : terreno(terreno_) { }
        double operator()(int id_origen, int id_destino) {
            int x_origen, y_origen;
            terreno.revertir_hash(id_origen, x_origen, y_origen);

            int x_destino, y_destino;
            terreno.revertir_hash(id_destino, x_destino, y_destino);
            
            int dx = x_destino - x_origen;
            int dy = y_destino - y_origen;

            return sqrt((dx * dx) + (dy * dy));
        }

        int ancho_terreno_;
        const Terreno& terreno;
    };
    
    CalcularDistancia distancia_entre(*this);
    AEstrella algoritmo(distancia_entre);
    
    int id_inicio = hash_posicion(inicio.x(), inicio.y());
    int id_fin = hash_posicion(fin.x(), fin.y());

    std::vector<int> camino = algoritmo.calcular_camino(this, 
        id_inicio, id_fin);
    
    std::vector<Posicion> resultado;

    for (auto it = camino.rbegin(); it != camino.rend(); ++it) {
        int x, y;
        revertir_hash(*it, x, y);

        resultado.push_back(Posicion((float)x, (float)y));
    }

    return resultado;
}

} // namespace modelo
