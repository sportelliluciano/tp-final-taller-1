#include "modelo/unidad.h"

#include <iostream>

#include "libs/json.hpp"

#include "modelo/arma.h"
#include "modelo/edificio.h"
#include "modelo/unidad_base.h"
#include "modelo/posicion.h"
#include "conexion/i_jugador.h"

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
    esta_en_camino = true;
}

bool Unidad::en_movimiento(){
    return esta_en_camino;
}

bool Unidad::llego_a(Posicion& posicion_) {
    return (abs(posicion_.x() - posicion.x()) < 1) && 
        (abs(posicion_.y() - posicion.y()) < 1);
}
void Unidad::actualizar_posicion(int dt,Terreno* terreno,
                    std::vector<IJugador*>& jugadores) {
    if (llego_a(camino[paso_actual])) {
        std::cout << "Llegue a la celda:  "<<camino[paso_actual].x()<<camino[paso_actual].y() << std::endl;
        //if (paso_actual > 0){
            for (auto it=jugadores.begin();it != jugadores.end();++it){
            (*it)-> sincronizar_tropa(0,
                                        posicion.x()*8,posicion.y()*8);
            }
            terreno->eliminar_tropa(posicion,unidad_base.get_dimensiones());
            terreno->agregar_tropa(posicion.x(),posicion.y(),unidad_base.get_dimensiones());
        //}
        paso_actual++;

        if (paso_actual >= camino.size()) {
            esta_en_camino = false;
            paso_actual = 0;
        } 
    }
    std::cout << "Antes de avanzar x: "<<posicion.x() <<" en px: "<<posicion.px_x() << std::endl;
    std::cout << "Antes de avanzar y: "<<posicion.y() <<" en px: "<<posicion.px_y() << std::endl;
    int x_destino = camino[paso_actual].x();
    int y_destino = camino[paso_actual].y();
    float vx = 0, vy = 0;
    std::cout << "Destino x: "<<x_destino << std::endl;
    std::cout << "Destino y: "<<y_destino << std::endl;
    if (abs(x_destino - posicion.x()) > 0) {
        vx = (x_destino - posicion.px_x()/8) / abs(x_destino - posicion.px_x()/8);
        vx *= 0.4 / 15;
    }

    if (abs(y_destino - posicion.y()) > 0) {
        vy = (y_destino - posicion.px_y()/8) / abs(y_destino - posicion.px_y()/8);
        vy *= 0.4 / 15;
    }

    float dx = vx * dt,
        dy = vy * dt;
    std::cout << "paso en x: "<<dx << std::endl;
    std::cout << "Paso en y: "<<dy << std::endl;
    if (abs(dx) > abs(posicion.px_x() - x_destino))
        posicion.actualizar_px_x(x_destino);
    else
        posicion.actualizar_px_x(posicion.px_x()+ dx);
        
    if (abs(dy) > abs(posicion.px_y() - y_destino))
        posicion.actualizar_px_y(y_destino);
    else
        posicion.actualizar_px_y(posicion.px_y()+ dy);
        
    posicion.actualizar(std::floor(posicion.px_x()/8),
                        std::floor(posicion.px_y()/8));
    std::cout << "Despues de avanzar x: "<<posicion.x() <<" en px: "<<posicion.px_x() << std::endl;
    std::cout << "Despues de avanzar y: "<<posicion.y() <<" en px: "<<posicion.px_y() << std::endl;                    
}
std::string& Unidad::get_clase() const {
    return unidad_base.get_clase();
}
}  // namespace modelo
