#include "modelo/terreno.h"

#include <iostream>

#include <cmath>
#include <list>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <unordered_set>
#include <limits>
#include <queue>

#include "modelo/a_estrella.h"
#include "modelo/celda.h"

// El tamaño de una celda [enunciado / dato de cliente] es de 32x32,
//  pero el tamaño de una celda del modelo es de 8x8 para poder ubicar 
//  una tropa por celda.
#define FACTOR_DIMENSION 4
#define RANGO 5 * FACTOR_DIMENSION

namespace modelo {

Terreno::Terreno() { }

void Terreno::inicializar(const nlohmann::json& mapa) {
    alto = ancho = 0;
    const std::vector<std::vector<int>>& tipos = 
        mapa.at("tipo").get<std::vector<std::vector<int>>>();

    alto = tipos.size() * FACTOR_DIMENSION;

    if (alto == 0) {
        throw std::runtime_error("Terreno inválido");
    }

    ancho = tipos.at(0).size() * FACTOR_DIMENSION;

    for (int y=0; y < alto; y++) {
        std::vector<Celda> fila_actual;
        for (int x=0; x < ancho; x++) {
            int x_tipo = x / FACTOR_DIMENSION;
            int y_tipo = y / FACTOR_DIMENSION;
            fila_actual.push_back(
                Celda((tipo_celda_t)tipos[y_tipo][x_tipo], x, y));
        }
        terreno.push_back(fila_actual);
    }
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
    int x, y;
    revertir_hash(nodo, x, y);
    
    std::vector<int> vecinos;

    // TODO: limpiar esto

    // Celdas que siempre son caminables (a menos que sean obstáculo)
    bool arriba = ((y - 1) > 0) && terreno[y-1][x].es_caminable(), 
         abajo  = ((y + 1) < alto) && terreno[y+1][x].es_caminable(), 
         izquierda = ((x - 1) > 0) && terreno[y][x-1].es_caminable(), 
         derecha = ((x + 1) < ancho) && terreno[y][x+1].es_caminable();
    // Celdas que son caminables sólo si las aledañas son caminables
    //  (y no son obstáculos)
    bool arriba_izq = (arriba && izquierda) && terreno[y-1][x-1].es_caminable(),
         arriba_der = (arriba && derecha) && terreno[y-1][x+1].es_caminable(),
         abajo_izq = (abajo && izquierda) && terreno[y+1][x-1].es_caminable(),
         abajo_der = (abajo && derecha) && terreno[y+1][x+1].es_caminable();

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

    return std::move(vecinos);
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

bool Terreno::puede_construir_edificio(int x, int y, std::pair<int,int>& dim) {
    int dim_x = dim.first;
    int dim_y = dim.second;
    int inicio_x = x-RANGO;
    if (inicio_x < 0) inicio_x = 0; 
    int inicio_y = y-RANGO;
    if (inicio_y < 0) inicio_y = 0;
    int fin_x = x+dim_x+RANGO;
    if (fin_x > ancho) fin_x = ancho;
    int fin_y = y+dim_y+RANGO;
    if (fin_y > alto) fin_y = alto;
    bool rango_valido = false;
    for (int j = inicio_y; j < fin_y; j++){
        for (int i = inicio_x; i < inicio_x; i++){
            if( j >= y && j< y+dim_y && i >= x && i< x+dim_x){
                if(!terreno[j][i].es_construible()) {
                   return false;    
                }
                continue;
            }
            if(terreno[j][i].tiene_edificio()) {
                rango_valido = true;
            }
        }
    }
    return rango_valido;
}

bool Terreno::rango_valido_tropa(int x_, int y_,std::pair<int,int>& dim) {
    int dim_x = dim.first;
    int dim_y = dim.second;
    for (int y = y_; y < y_ + dim_y; y++){
        for (int x = x_; x < x_ + dim_x; x++) {
            Celda& celda = terreno.at(y).at(x);
            if (!celda.es_caminable()) {
                return false;
            }
        }
    }
    return true;
}

bool Terreno::tiene_edificio(int x_, int y_) {
    return terreno.at(x_).at(y_).tiene_edificio();
}

void Terreno::agregar_edificio(int x_, int y_,std::pair<int,int>& dim){
    int dim_x = dim.first * FACTOR_DIMENSION;
    int dim_y = dim.second * FACTOR_DIMENSION;
    for (int j = y_; j < y_ + dim_y; j++) {
        for (int i = x_; i < x_ + dim_x; i++) {
            terreno[j][i].agregar_edificio();
        }
    }
}

void Terreno::eliminar_edificio(Posicion& pos,std::pair<int,int>& dim) {
    int x_ = pos.x();
    int y_ = pos.y();
    int dim_x = dim.first * FACTOR_DIMENSION;
    int dim_y = dim.second * FACTOR_DIMENSION;
    for (int j = y_; j < y_ + dim_y; j++) {
        for (int i = x_; i < x_ + dim_x; i++) {
            terreno[j][i].eliminar_edificio();
        }
    }
}

bool Terreno::es_caminable(int x, int y) {
    return terreno[y][x].es_caminable();
}

bool Terreno::hay_tropa(int x, int y) {
    return terreno[y][x].hay_tropa();
}

void Terreno::agregar_tropa(const Posicion& posicion, std::pair<int,int>& dim) {
    int dim_x = dim.first;
    int dim_y = dim.second;
    for (int j = posicion.y(); j < posicion.y() + dim_y; j++){
        for (int i = posicion.x(); i < posicion.x() + dim_x; i++){
            terreno[j][i].agregar_tropa();
        }
    }
}

void Terreno::eliminar_tropa(const Posicion& pos, std::pair<int,int>& dim) {
    int dim_x = dim.first;
    int dim_y = dim.second;
    for (int j = pos.y(); j < pos.y() + dim_y; j++){
        for (int i = pos.x(); i < pos.x() + dim_x; i++){
            terreno[j][i].eliminar_tropa();
        }
    }
}
Posicion Terreno::obtener_especia_cercana(Posicion& posicion_i) {
    Posicion celda;
    bool encontre = false;
    float distancia_minima = std::numeric_limits<float>::infinity();
     for (int j = 0; j < alto; j++){
        for (int i = 0; i < ancho; i++){
            Celda& celda_actual = terreno[j][i]; 
            if(celda_actual.tipo()==4) {
                encontre = true;
                Posicion pos_actual(celda_actual.x(),celda_actual.y());
                float distancia = posicion_i.distancia_a(pos_actual);
                if (distancia < distancia_minima){
                    distancia_minima = distancia;
                    celda =  pos_actual;
                }
            }
        }
    }
    if (!encontre)
        throw std::runtime_error("no hay especia");
    return celda;
}
/*
Posicion Terreno::obtener_especia_cercana(Posicion& posicion_i) {
    std::list<int> cola;
    std::unordered_set<int> visitados;
    cola.push_back(hash_posicion(posicion_i.x(), posicion_i.y()));
    while (!cola.empty()) {
        int nodo = cola.front();
        visitados.insert(nodo);
        cola.pop_front();
        
        int x, y;
        revertir_hash(nodo, x, y);
        if (terreno[y][x].tipo()==4)
            return Posicion(terreno[y][x].x(), terreno[y][x].y());
        
        for (int vecino : obtener_vecinos(nodo)) {
            if (visitados.find(vecino) != visitados.end())
                continue;
            cola.push_back(vecino);
        }
    }

    throw std::runtime_error("No hay más posiciones libres en el tablero.");
}
*/

bool tiene(std::vector<Posicion>& visitados, Posicion& vecino_pos){
    for (auto it = visitados.begin(); it != visitados.end();++it){
        if (*it == vecino_pos)
            return true;
    }
    return false;
}
/*
Posicion Terreno::obtener_especia_cercana(Posicion& posicion_i) {
    std::queue<Posicion> cola;
    std::vector<Posicion> visitados;
    cola.emplace(posicion_i);
    std::cout << "entre al obtener_especia"<<std::endl;
    while (!cola.empty()) {
        Posicion posicion = cola.front(); 
        cola.pop();
        visitados.push_back(posicion);
        if (es_especia(posicion)){
            std::cout << "sali bien"<<std::endl;
            return posicion;
        }
        int nodo = hash_posicion(posicion.x(),posicion.y());    
        for (int vecino : obtener_vecinos(nodo)) {
            int x,y;
            revertir_hash(vecino,x,y);
            Posicion vecino_pos(x,y);
            if (tiene(visitados,vecino_pos))
                continue;
            cola.push(vecino_pos);
        }
    }

    throw std::runtime_error("No hay más posiciones libres en el tablero.");
}
*/
Posicion Terreno::obtener_posicion_caminable_cercana(
        const Posicion& posicion_inicial) {
    for (int j = posicion_inicial.y(); j <alto; j++){
        for (int i = posicion_inicial.x(); i < ancho; i++){
            if(!terreno[j][i].es_caminable())
                continue;
            return Posicion(terreno[j][i].x(),terreno[j][i].y());
        }
    }
    // TODO: esto sólo camina hacia adelante, estando sobre el final 
    // del terreno puede fallar
    throw std::runtime_error("wut");
}

void Terreno::agregar_refineria(int x, int y,int id_jugador) {
    std::cout << "Creada refineria para " << id_jugador << "en (" << x 
              << ";" << y << ")" << std::endl;
    if (refinerias.count(id_jugador)==0){
        refinerias[id_jugador] = std::vector<Posicion>();
    }
    refinerias.at(id_jugador).push_back(Posicion(x,y));
}
void Terreno::eliminar_refineria(const Posicion& posicion,int id_jugador) {
    std::cout << "Eliminada refineria para " << id_jugador << "en ("
              << posicion.x() << ";" << posicion.y() << ")" << std::endl;
    std::vector<Posicion>& refinerias_jugador = refinerias.at(id_jugador);
    for (auto it = refinerias_jugador.begin(); it != refinerias_jugador.end(); ++it) {
        Posicion& pos_refineria = *it;
        if (pos_refineria == posicion) {
            refinerias_jugador.erase(it);
            return;
        }
    }
}


void Terreno::agregar_centro(int x, int y,int id_jugador) {
    centros.emplace(id_jugador,Posicion(x,y));
}
void Terreno::eliminar_centro(int id_jugador) {
    if (centros.count(id_jugador != 0))
        centros.erase(id_jugador);
}
Posicion& Terreno::obtener_centro_posicion(int id_jugador){
    if (centros.count(id_jugador != 0))
        centros.at(id_jugador);
}

//futura mejora: usar un heap
Posicion Terreno::obtener_refinerias_cercana(const Posicion& pos,int id_jugador) {
    if (refinerias.count(id_jugador)==0)
        throw std::runtime_error("no hay refinerias");
    std::vector<Posicion>& refinerias_ = refinerias.at(id_jugador);
    if (refinerias_.empty())
        throw std::runtime_error("no hay refinerias");
    std::cout << "al buscar refnerias hay  "<<refinerias_.size() <<" refinerias" << std::endl;    
    float distancia_minima = std::numeric_limits<float>::infinity();
    Posicion min_pos = refinerias_.front();
    for (auto refineria = refinerias_.begin();refineria!=refinerias_.end();++refineria){
        float distancia = pos.distancia_a(*refineria);
        if (distancia < distancia_minima){
            min_pos = *refineria;
            distancia_minima = distancia;
        }
    }
    return min_pos;
}

bool Terreno::es_especia(const Posicion& posicion){
   int j = posicion.y();
    int i = posicion.x();
    if (terreno[j][i].tipo() == 4)//especia
        return true;
    return false;
}

} // namespace modelo
