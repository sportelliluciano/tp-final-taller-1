#include "modelo/unidad.h"

#include "libs/json.hpp"

#include "modelo/arma.h"
#include "modelo/edificio.h"
#include "modelo/unidad_base.h"
#include "modelo/posicion.h"
#include "modelo/i_jugador.h"

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
void Unidad::mover(int x, int y){
    posicion.actualizar(x,y);
}
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
unsigned int Unidad::x(){
    return posicion.x();
}
unsigned int Unidad::y(){
    return posicion.y();
}
void Unidad::configurar_camino(std::vector<Posicion> nuevo_camino){
    camino = nuevo_camino;
    esta_en_camino = true;
}

bool Unidad::en_movimiento(){
    return esta_en_camino;
}

bool Unidad::llego_a(Posicion& posicion_) {
    return (abs(posicion_.x() - posicion.x()) < 1) && 
        (abs(posicion_.y() - posicion.y()) < 1);
}
void Unidad::actualizar_posicion(int dt,IJugador* jugador) {
    if (esta_en_camino) {
        if (llego_a(camino[paso_actual])) {
            if (paso_actual > 0)
                jugador->sincronizar_tropa(0, posicion.x(), posicion.y());
                
            paso_actual++;

            if (paso_actual >= camino.size()) {
                esta_en_camino = false;
                paso_actual = 0;
            } 
        }
    }
    int x_destino = camino[paso_actual].x();
    int y_destino = camino[paso_actual].y();
    float vx = 0, vy = 0;

    if (abs(x_destino - posicion.x()) > 0) {
        vx = (x_destino - posicion.px_x()) / abs(x_destino - posicion.px_x());
        vx *= 0.4 / 15;
    }

    if (abs(y_destino - posicion.y()) > 0) {
        vy = (y_destino - posicion.y()) / abs(y_destino - posicion.y());
        vy *= 0.4 / 15;
    }

    float dx = vx * dt,
        dy = vy * dt;

    if (abs(dx) > abs(posicion.px_x() - x_destino))
        posicion.actualizar_px_x(x_destino);
    else
        posicion.actualizar_px_x(posicion.px_x()+ dx);
        
    if (abs(dy) > abs(posicion.px_y() - y_destino))
        posicion.actualizar_px_y(y_destino);
    else
        posicion.actualizar_px_y(posicion.px_y()+ dy);
        
    posicion.actualizar(std::floor(posicion.px_x()),
                        std::floor(posicion.px_y()));
}
}  // namespace modelo
