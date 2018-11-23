#include "cosechadora.h"

#define CONSTANTE_VELOCIDAD ((0.4f / 15) / 16)

namespace modelo {

Cosechadora::Cosechadora(int id_,int pos_x,int pos_y, UnidadBase& unidad_base_
    ,Terreno* terreno):
    Unidad(id_, pos_x, pos_y, unidad_base_),
    terreno_(terreno){    
    
}
Cosechadora::~Cosechadora(){

}

bool Cosechadora::camino_a_especia(){
    return camino_especia;
}
void Cosechadora::operar(int ds){
    tiempo_descarga -= ds;
    if (tiempo_descarga <= 0){
        operando_ = false;
    }
    tiempo_descarga = ESPERA;
}
bool Cosechadora::operando(){
    return operando_;
}
//IJugador* Cosechadora::obtener_jugador(){
//    return jugador;
//}
bool Cosechadora::actualizar_posicion(int dt, Terreno* terreno) {
    if (!esta_en_camino)
        return false;
    
    bool resincronizar = false;
    if (posicion == camino[paso_actual]) {
        resincronizar = true;
        paso_actual++;

        if (paso_actual >= camino.size()) {
            //llegue
            operando_ = true;
            camino_especia = !camino_especia;

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
}