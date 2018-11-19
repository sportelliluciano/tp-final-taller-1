#include "cosechadora.h"

namespace modelo {

Cosechadora::Cosechadora(int id_,int pos_x,int pos_y, UnidadBase& unidad_base_
    ,Terreno& terreno,IJugador* jugador_):
    Unidad(id_, pos_x, pos_y, unidad_base_),
    terreno_(terreno),
    jugador(jugador_){    
    
}
Cosechadora::~Cosechadora(){

}

bool Cosechadora::camino_a_especia(){
    return camino_especia;
}

IJugador* Cosechadora::obtener_jugador(){
    return jugador;
}

void Cosechadora::actualizar_posicion(int dt,Terreno terreno,
                    std::vector<IJugador*>& jugadores) {
    if (llego_a(camino[paso_actual])) {
        //if (paso_actual > 0){
            for (auto it=jugadores.begin();it != jugadores.end();++it){
            (*it)-> sincronizar_tropa(0,
                                        posicion.x(),posicion.y());
            }
            terreno.eliminar_tropa(posicion,unidad_base.get_dimensiones());
            terreno.agregar_tropa(posicion.x(),posicion.y(),unidad_base.get_dimensiones());
        //}
        paso_actual++;

        if (paso_actual >= camino.size()) {
            esta_en_camino = false;
            camino_especia = !camino_especia;
            paso_actual = 0;
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
}