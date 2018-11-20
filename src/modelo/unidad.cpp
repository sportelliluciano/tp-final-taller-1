#include "modelo/unidad.h"

#include <iostream>

#include "libs/json.hpp"

#include "modelo/arma.h"
#include "modelo/edificio.h"
#include "modelo/unidad_base.h"
#include "modelo/posicion.h"

namespace modelo {

Unidad::Unidad(int id_, int x, int y, UnidadBase& unidad_base_):
    id(id_),
    posicion(Posicion(x,y)),
    vida(unidad_base_.get_vida()),
    unidad_base(unidad_base_)
{
    //se le asigna un id unico
}
Unidad::~Unidad() {

}/*
void Unidad::mover(int x, int y){
    posicion.actualizar(x,y);
}
*/
void Unidad::recibir_dano(unsigned int dano){
    vida -= dano;
    //if (vida <= 0) destruir
}

void Unidad::atacar(Unidad& victima){
    unidad_base.atacar_a(&victima);
}

void Unidad::atacar(Edificio& edificio){
    unidad_base.atacar_a(&edificio);
}

int Unidad::get_id(){
    return id;
}
unsigned int Unidad::get_vida(){
    return vida;
}
std::pair<int,int>& Unidad::get_dimensiones() {
    return unidad_base.get_dimensiones();
}
Posicion& Unidad::get_posicion(){
    return posicion;
}
/*
unsigned int Unidad::x(){
    return posicion.x();
}
unsigned int Unidad::y(){
    return posicion.y();
}*/
void Unidad::configurar_camino(std::vector<Posicion> nuevo_camino){
    camino = nuevo_camino;
    paso_actual = 1;
    esta_en_camino = true;
}

bool Unidad::en_movimiento(){
    return esta_en_camino;
}

bool Unidad::llego_a(Posicion& posicion_) {
    int x_destino = posicion_.px_x();
    int y_destino = posicion_.px_y();
    int x_actual = posicion.px_x();
    int y_actual = posicion.px_y();
    return (x_destino == x_actual) && (y_destino == y_actual);
}

bool Unidad::actualizar_posicion(int dt, Terreno* terreno) {
    if (!esta_en_camino)
        return false;
    
    bool resincronizar = false;
    if (llego_a(camino[paso_actual])) {
        terreno->eliminar_tropa(posicion,unidad_base.get_dimensiones());
        terreno->agregar_tropa(posicion.x(),posicion.y(),unidad_base.get_dimensiones());
        resincronizar = true;
        paso_actual++;

        if (paso_actual >= camino.size()) {
            esta_en_camino = false;
            paso_actual = 0;
            return resincronizar;
        }
    }

    float velocidad = 16; // Velocidad de la tropa en unidades del enunciado

    int x_destino = camino[paso_actual].px_x();
    int y_destino = camino[paso_actual].px_y();
    int x_actual = posicion.px_x();
    int y_actual = posicion.px_y();
    float fx_actual = posicion.px_x();
    float fy_actual = posicion.px_y();
    int vx = 0, vy = 0;

    if (x_destino != x_actual)
        vx = x_destino - x_actual;
    if (y_destino != y_actual)
        vy = y_destino - y_actual;

    if ((vx == 0) && (vy == 0))
        return false;
    
    float veloc_x = vx / sqrt(vx*vx + vy*vy),
          veloc_y = vy / sqrt(vx*vx + vy*vy);
    
    veloc_x *= ((0.4 / 15) / 16) * unidad_base.get_velocidad();
    veloc_y *= ((0.4 / 15) / 16) * unidad_base.get_velocidad();

    float dx = veloc_x * dt,
          dy = veloc_y * dt;

    if (abs(dx) < abs(x_destino - fx_actual))
        fx_actual += dx;
    else
        fx_actual = x_destino;
        
    if (abs(dy) < abs(y_destino - fy_actual))
        fy_actual += dy;
    else
        fy_actual = y_destino;

    posicion.actualizar_px_x(fx_actual);
    posicion.actualizar_px_y(fy_actual);
    return resincronizar;
}

std::string& Unidad::get_clase() const {
    return unidad_base.get_clase();
}

}  // namespace modelo

