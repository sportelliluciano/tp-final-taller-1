#include "cliente/modelo/disparo_proyectil.h"

#include "cliente/modelo/disparo.h"
#include "cliente/video/camara.h"
#include "cliente/video/posicion.h"
#include "cliente/video/ventana.h"

#define MS_POR_SEG 1000
#define CONSTANTE_VELOCIDAD ((0.4 / 15) / 16)

namespace cliente {

DisparoProyectil::DisparoProyectil(float frecuencia_disparo) 
: periodo(MS_POR_SEG / frecuencia_disparo) 
{
    tiempo_para_impacto = 0;
    velocidad = 1.0f;
    orientacion = 0;
}

void DisparoProyectil::iniciar(int x_origen, int y_origen, int x_destino, 
    int y_destino) 
{
    esta_activo = true;
    esta_explotando = false;
    esta_viajando = false;
    origen = Posicion(x_origen, y_origen);
    pos_actual = origen;
    fx_actual = x_origen;
    fy_actual = y_origen;
    tiempo_para_impacto = 0;
    
    DisparoProyectil::actualizar_destino(x_destino, y_destino);
}

void DisparoProyectil::actualizar_destino(int x_destino, int y_destino) {
    pos_victima.x = x_destino;
    pos_victima.y = y_destino;
    int vx = pos_victima.x - pos_actual.x, vy = pos_victima.y - pos_actual.y;
    if (periodo != tiempo_para_impacto) {
        velocidad = (MS_POR_SEG * sqrt(vx*vx + vy*vy)) / 
            (periodo - tiempo_para_impacto);
    }
}

void DisparoProyectil::detener() {
    esta_activo = false;
}

void DisparoProyectil::actualizar_orientacion() {
    int vx = pos_victima.x - pos_actual.x, 
        vy = pos_victima.y - pos_actual.y;
    
    if (vx > 0) { vx = 1; } else if (vx < 0) { vx = -1; }
    if (vy > 0) { vy = 1; } else if (vy < 0) { vy = -1; }
    
    int pos_sprite = 0;
    
    switch(vx) {
        case -1:
        switch(vy) {
            case -1: pos_sprite = 7; break;
            case  0: pos_sprite = 6; break;
            case  1: pos_sprite = 5; break;
        }
        break;

        case  0:
        switch(vy) {
            case -1: pos_sprite = 0; break;
            case  0: pos_sprite = 0; break;
            case  1: pos_sprite = 4; break;
        }
        break;

        case  1:
        switch(vy) {
            case -1: pos_sprite = 1; break;
            case  0: pos_sprite = 2; break;
            case  1: pos_sprite = 3; break;
        }
        break;
    }
    
    orientacion = pos_sprite;
}

void DisparoProyectil::actualizar_movimiento(int dt_ms) {
    if (pos_victima == pos_actual)
        return;

    int vx = pos_victima.x - pos_actual.x, vy = pos_victima.y - pos_actual.y;

    if ((vx == 0) && (vy == 0))
        return;

    float k = CONSTANTE_VELOCIDAD * velocidad / sqrt(vx*vx + vy*vy);
    float veloc_x = k * vx,
          veloc_y = k * vy;

    float dx = veloc_x * dt_ms,
          dy = veloc_y * dt_ms;

    if (abs(dx) < abs(pos_victima.x - fx_actual))
        fx_actual += dx;
    else
        fx_actual = pos_victima.x;
        
    if (abs(dy) < abs(pos_victima.y - fy_actual))
        fy_actual += dy;
    else
        fy_actual = pos_victima.y;
    
    pos_actual.x = round(fx_actual);
    pos_actual.y = round(fy_actual);
    
    actualizar_orientacion();
}

void DisparoProyectil::actualizar(int dt_ms) {
    if (esta_explotando)
        return;
    
    if (esta_activo && !esta_viajando) {
        // Crear nuevo misil
        pos_actual = origen;
        fx_actual = origen.x;
        fy_actual = origen.y;
        esta_viajando = true;
    } else if (esta_viajando) {
        // Interpolar la posición
        actualizar_movimiento(dt_ms);
        if ((pos_actual == pos_victima) || (tiempo_para_impacto > periodo)) {
            esta_viajando = false;
            esta_explotando = true;
            tiempo_para_impacto = 0;
        }
        tiempo_para_impacto += dt_ms;
    }
}

} // namespace cliente
