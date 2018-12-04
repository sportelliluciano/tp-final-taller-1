#include "modelo/unidad.h"

#include <iostream>
#include <utility>
#include <vector>
#include <string>

#include "libs/json.hpp"

#include "modelo/arma.h"
#include "modelo/edificio.h"
#include "modelo/unidad_base.h"
#include "modelo/posicion.h"

// Esta constante se determinó de forma empírica.
#define CONSTANTE_VELOCIDAD ((0.4f / 15) / 16)

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
}
//false = muerto ,true= vivo
int Unidad::recibir_dano(unsigned int dano){
    vida -= dano;
    return vida;
}

int Unidad::atacar(Atacable* victima_){
    if (posicion.distancia_celda_a(victima_->get_posicion(),
            victima_->get_dimensiones())>unidad_base.get_rango())
        throw std::runtime_error("Fuera de rango");
    return unidad_base.atacar_a(victima_);
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

void Unidad::configurar_camino(const std::vector<Posicion>& nuevo_camino) {
    camino = nuevo_camino;
    paso_actual = 0;
    esta_en_camino = true;
}

bool Unidad::en_movimiento() const {
    return esta_en_camino;
}

bool Unidad::actualizar_posicion(int dt, Terreno* terreno) {
    if (!esta_en_camino)
        return false;
    bool resincronizar = false;
    if (posicion == camino[paso_actual]) {
        resincronizar = true;
        paso_actual++;
        if (paso_actual >= camino.size()) {
            esta_en_camino = false;
            paso_actual = 0;
            return resincronizar;
        }
    }

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
    
    terreno->eliminar_tropa(posicion,unidad_base.get_dimensiones());
    float veloc_x = vx / sqrt(vx*vx + vy*vy),
          veloc_y = vy / sqrt(vx*vx + vy*vy);
    
    veloc_x *= CONSTANTE_VELOCIDAD * unidad_base.get_velocidad();
    veloc_y *= CONSTANTE_VELOCIDAD * unidad_base.get_velocidad();

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
    terreno->agregar_tropa(posicion,unidad_base.get_dimensiones());
    return resincronizar;
}

bool Unidad::configurar_ataque(Atacable* victima_){
    if (posicion.distancia_celda_a(victima_->get_posicion(),
            victima_->get_dimensiones())>unidad_base.get_rango())
        return false;
    victima = victima_;
    atacando = true;
    tiempo_para_atacar = 0;
    id_victima_ = victima->get_id();
    return true;
}

int Unidad::actualizar_ataque(int dt, Terreno* terreno) {
    tiempo_para_atacar+=dt;
    if (!victima) return 0;//esta muerta
    if (tiempo_para_atacar >= 1000/(unidad_base.obtener_frecuencia())){
        tiempo_para_atacar = 0;
    }
    return (atacar(victima));
}

std::string& Unidad::get_clase() const {
    return unidad_base.get_clase();
}

bool Unidad::esta_atacando(){
    return atacando;
}

void Unidad::parar_ataque(){
    atacando = false;
    victima = nullptr;
    tiempo_para_atacar=0;
    id_victima_ = -1;
}

int Unidad::id_victima(){
    return id_victima_;
}
}  // namespace modelo
