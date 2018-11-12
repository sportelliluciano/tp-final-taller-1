#include "modelo/terreno.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "modelo/a_estrella.h"
#include "modelo/celda.h"

#define RANGO 5

namespace modelo {

Terreno::Terreno(const char *ruta_csv) {
    std::ifstream entrada(ruta_csv);
    if (!entrada.good())
        throw std::runtime_error("No se pudo abrir el archivo de terreno");
    
    int x = 0, y = 0;
    alto = ancho = 0;

    while (!entrada.eof()) {
        std::string linea, tile;
        std::getline(entrada, linea);
        std::stringstream s_linea(linea);
        
        std::vector<Celda> fila_actual;
        
        while (std::getline(s_linea, tile, ';')) {
            std::stringstream s_tile(tile);
            int tile_no = 0;
            if (!(s_tile >> tile_no))
                throw std::runtime_error("Archivo de terreno inválido");
            
            tipo_celda_t tipo = CELDA_ARENA;
            if (tile_no == 1)
                tipo = CELDA_ROCA;

            fila_actual.push_back(Celda(tipo,x,y));
            
            if (x > ancho)
                ancho = x;
            
            x++;
        }
        
        if (y > alto)
            alto = y;

        x = 0;
        y++;
        terreno.push_back(fila_actual);
    }
}

Celda& Terreno::obtener_celda(int x, int y) {
    return terreno[x][y];
}

int Terreno::obtener_ancho() const {
    return ancho;
}

int Terreno::obtener_alto() const {
    return alto;
}

int Terreno::hash_posicion(int x, int y) const {
    // Mientras esto no de overflow el hash no colisiona.
    // TODO: Mejorar esto?
    return (y * ancho) + x;
}

void Terreno::revertir_hash(int hash, int& x, int& y) const {
    x = hash % ancho;
    y = hash / ancho;
}

// Esta función es más compleja de lo que parece.
// Devuelve los vecinos a la celda actual teniendo en cuenta que:
// - Sólo se puede ir en diagonal si las dos diagonales opuestas no están 
//   bloqueadas
std::vector<int> Terreno::obtener_vecinos(int nodo) const {
    std::cout << "Entre a la etapa 6 " << '\n';
    int x, y;
    revertir_hash(nodo, x, y);
    
    std::vector<int> vecinos;

    // TODO: limpiar esto

    // Celdas que siempre son caminables (a menos que sean obstáculo)
    std::cout << "Etapa 6.1 " << '\n';
    bool arriba = ((y - 1) > 0) && terreno[x][y - 1].es_caminable(), 
         abajo  = ((y + 1) < alto) && terreno[x][y + 1].es_caminable(), 
         izquierda = ((x - 1) > 0) && terreno[x - 1][y].es_caminable(), 
         derecha = ((x + 1) < ancho) && terreno[x + 1][y].es_caminable();
    std::cout << "Etapa 6.2 " << '\n';    
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
    Terreno::buscar_camino_minimo(const Posicion& inicio, const Posicion& fin) const {
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
    std::cout << "Etapa 1 " << '\n';
    CalcularDistancia distancia_entre(*this);
    std::cout << "Etapa 2 " << '\n';
    AEstrella algoritmo(distancia_entre);
    std::cout << "Etapa 3 " << '\n';
    int id_inicio = hash_posicion(inicio.x(), inicio.y());
    std::cout << "Etapa 4 " << '\n';
    int id_fin = hash_posicion(fin.x(), fin.y());
    std::cout << "Etapa 5 " << '\n';
    std::vector<int> camino = algoritmo.calcular_camino(this, 
        id_inicio, id_fin);
    std::cout << "Etapa 6 " << '\n';  
    std::vector<Posicion> resultado;

    for (auto it = camino.rbegin(); it != camino.rend(); ++it) {
        int x, y;
        revertir_hash(*it, x, y);

        resultado.push_back(Posicion((float)x, (float)y));
    }
    std::cout << "Etapa 7 " << '\n';
    return resultado;
}

bool Terreno::rango_valido_edificio(int x_, int y_,std::pair<int,int>& dim){
    int dim_x = dim.first;
    int dim_y = dim.second;
    int inicio_x = x_-RANGO;
    if (inicio_x < 0) inicio_x = 0; 
    int inicio_y = y_-RANGO;
    if (inicio_y < 0) inicio_y = 0;
    int fin_x = x_+dim_x+RANGO;
    if (fin_x > ancho) fin_x = ancho;
    int fin_y = y_+dim_y+RANGO;
    if (fin_y > alto) fin_y = alto;
    bool rango_valido = false;
    for (int j = inicio_y; j < fin_y; j++){
        for (int i = inicio_x; i < inicio_x; i++){
            if( j >= y_ && j< y_+dim_y && i >= x_ && i< x_+dim_x){
                if(terreno[j][i].tiene_edificio() ||
                   terreno[j][i].hay_tropa() ||
                   !terreno[j][i].es_construible()){
                   return false;    
                }
                continue;
            }
            if(terreno[j][i].tiene_edificio()){
                   rango_valido = true;
            }
        }
    }
    return rango_valido;
}
bool Terreno::rango_valido_tropa(int x_, int y_,std::pair<int,int>& dim){
    int dim_x = dim.first;
    int dim_y = dim.second;
    for (int j = y_; j < y_ + dim_y; j++){
        for (int i = x_; i < x_ + dim_x; i++){
            if(terreno[j][i].tiene_edificio() ||
               terreno[j][i].hay_tropa() ||
               !terreno[j][i].es_caminable()){
                   return false;
        }       }
    }
    return true;
}
bool Terreno::tiene_edificio(int x_, int y_){
    return terreno[x_][y_].tiene_edificio();
}

void Terreno::agregar_edificio(int x_, int y_,std::pair<int,int>& dim){
    int dim_x = dim.first;
    int dim_y = dim.second;
    for (int j = y_; j < y_ + dim_y; j++){
        for (int i = x_; i < x_ + dim_x; i++){
            terreno[j][i].agregar_edificio();
        }
    }
}

void Terreno::eliminar_edificio(Posicion& pos,std::pair<int,int>& dim){
    int x_ = pos.x();
    int y_ = pos.y();
    int dim_x = dim.first;
    int dim_y = dim.second;
    for (int j = y_; j < y_ + dim_y; j++){
        for (int i = x_; i < x_ + dim_x; i++){
            terreno[j][i].eliminar_edificio();
        }
    }
}

bool Terreno::es_caminable(int x_, int y_){
    return terreno[x_][y_].es_caminable();
}

bool Terreno::hay_tropa(int x_, int y_){
    return terreno[x_][y_].hay_tropa();
}

void Terreno::agregar_tropa(int x_, int y_,std::pair<int,int>& dim){
    int dim_x = dim.first;
    int dim_y = dim.second;
    for (int j = y_; j < y_ + dim_y; j++){
        for (int i = x_; i < x_ + dim_x; i++){
            terreno[j][i].agregar_tropa();
        }
    }
}

void Terreno::eliminar_tropa(Posicion& pos,std::pair<int,int>& dim){
    int x_ = pos.x();
    int y_ = pos.y();
    int dim_x = dim.first;
    int dim_y = dim.second;
    for (int j = y_; j < y_ + dim_y; j++){
        for (int i = x_; i < x_ + dim_x; i++){
            terreno[j][i].eliminar_tropa();
        }
    }
}

} // namespace modelo
