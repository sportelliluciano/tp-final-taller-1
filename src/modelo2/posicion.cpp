#include "modelo/posicion.h"

#include <cmath>
#include <limits>

namespace modelo {

Posicion::Posicion() : x_(0), y_(0) { }

Posicion::Posicion(float x, float y) : x_(x*8), y_(y*8) { }

Posicion Posicion::operator+(const Posicion& otro) const {
    Posicion resultado(x_ + otro.x_, y_ + otro.y_);
    return resultado;
}

bool Posicion::operator==(const Posicion& otro) const {
    if (abs(x_ - otro.x_) > POSICION_CMP_TOLERANCIA)
        return false;
    if (abs(y_ - otro.y_) > POSICION_CMP_TOLERANCIA)
        return false;
    
    return true;
}

bool Posicion::operator!=(const Posicion& otro) const {
    return !operator==(otro);
}

int Posicion::distancia_celda_a(const Posicion& otro,std::pair<int,int>& dimension) const {
    float delta_x, delta_y;
    float distancia_minima = std::numeric_limits<float>::infinity();
    int alto = dimension.first;
    int ancho = dimension.second;
    for (int j = otro.y(); j< otro.y()+alto;j++){
        if (j == otro.y()||j==otro.y()+(alto-1)){
            for (int i = otro.x(); i<otro.x()+ancho;i++){
                delta_x = this->x() - i;
                delta_y = this->y() - j;
                float distancia = sqrt((delta_x * delta_x) + (delta_y * delta_y));
                if (distancia < distancia_minima)
                    distancia_minima = distancia;        
            }
        }
        delta_x = this->x() - otro.x();
        delta_y = this->y() - j;
        float distancia = sqrt((delta_x * delta_x) + (delta_y * delta_y));
        if (distancia < distancia_minima)
            distancia_minima = distancia;
        delta_x = this->x() - (otro.x()+(ancho-1));
        delta_y = this->y() - j;
        distancia = sqrt((delta_x * delta_x) + (delta_y * delta_y));
        if (distancia < distancia_minima)
            distancia_minima = distancia;
    }
    return distancia_minima;
}
float Posicion::distancia_a(const Posicion& otro) const{
    int delta_x, delta_y;
    delta_x = x_ - otro.x_;
    delta_y = y_ - otro.y_;
    return sqrt((delta_x * delta_x) + (delta_y * delta_y));
}

float Posicion::x() const {
    return x_ / 8;
}

float Posicion::y() const {
    return y_ / 8;
}

void Posicion::actualizar(float x,float y) {
    x_ = x * 8;
    y_ = y * 8;
}

float Posicion::px_x() const {
    return x_;
}

float Posicion::px_y() const {
    return y_;
}

void Posicion::actualizar_px_x(float x) {
    x_ = x;
}

void Posicion::actualizar_px_y(float y) {
    y_ = y;
}

} // namespace modelo
