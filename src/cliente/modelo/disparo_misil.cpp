#include "cliente/modelo/disparo_misil.h"

#include "cliente/modelo/disparo.h"
#include "cliente/modelo/sprite_animado.h"
#include "cliente/video/camara.h"
#include "cliente/video/posicion.h"
#include "cliente/video/ventana.h"

namespace cliente {

DisparoMisil::DisparoMisil() {
    explosion = SpriteAnimado(3634, 3638);
    explosion.set_centrado(true);
    for (int i=0;i<32;i++) {
        std::vector<Sprite> cuadros;
        for (int j=0;j<5;j++) {
            cuadros.push_back(Sprite(3336 + 32*j + i));
        }
        misil[i] = SpriteAnimado(cuadros);
        misil[i].configurar_repeticion(true);
        misil[i].set_centrado(true);
    }
}

void DisparoMisil::iniciar(int x_origen, int y_origen, int x_destino, 
        int y_destino) 
{
    esta_activo = true;
    esta_explotando = false;
    esta_viajando = false;
    origen = Posicion(x_origen, y_origen);
    pos_actual = origen;
    fx_actual = x_origen;
    fy_actual = y_origen;
    pos_victima = Posicion(x_destino, y_destino);
}

void DisparoMisil::actualizar_destino(int x_destino, int y_destino) {
    pos_victima.x = x_destino;
    pos_victima.y = y_destino;
}

void DisparoMisil::detener() {
    esta_activo = false;
}

static int calcular_orientacion(const Posicion& actual, const Posicion& destino) 
{
    int vx = destino.x - actual.x;
    int vy = destino.y - actual.y;
    if (vx > 0)
        vx = 1;
    else if (vx < 0)
        vx = -1;
    if (vy > 0)
        vy = 1;
    else if (vy < 0)
        vy = -1;
    int pos_sprite = 0;
    switch(vx) {
        case -1:
        switch(vy) {
            case -1:
                pos_sprite = 7; //28;
                break;
            case  0:
                pos_sprite = 6; //24;
                break;
            case  1:
                pos_sprite = 5; //20;
                break;
        }
        break;

        case  0:
        switch(vy) {
            case -1:
                pos_sprite = 0; // 0; 
                break;
            case  0:
                pos_sprite = 0;
                break;
            case  1:
                pos_sprite = 4; //16;  
                break;
        }
        break;

        case  1:
        switch(vy) {
            case -1:
                pos_sprite = 1; //4;
                break;
            case  0:
                pos_sprite = 2; //8;
                break;
            case  1:
                pos_sprite = 3; //12;
                break;
        }
        break;
    }
    
    return pos_sprite * 4;
}

#define CONSTANTE_VELOCIDAD ((0.4 / 15) / 16)
void DisparoMisil::actualizar_movimiento(int dt_ms) {
    if (pos_victima == pos_actual)
        return;

    int vx = pos_victima.x - pos_actual.x, vy = pos_victima.y - pos_actual.y;
    
    float velocidad = 32;

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
}

void DisparoMisil::actualizar(int dt_ms) {
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
        if (pos_actual == pos_victima) {
            esta_viajando = false;
            esta_explotando = true;
        }
    }
}

void DisparoMisil::renderizar(Ventana& ventana, Camara& camara) {
    if (!esta_activo && !esta_viajando && !esta_explotando)
        return;
    
    Posicion visual = camara.traducir_a_visual(pos_actual);

    if (esta_viajando) {
        // mostrar en el aire    
        int orientacion = calcular_orientacion(pos_actual, pos_victima);
        misil[orientacion].renderizar(ventana, visual.x, visual.y);
    } else if (esta_explotando) {
        explosion.renderizar(ventana, visual.x, visual.y);
        if (explosion.finalizado()) {
            esta_explotando = false;
            explosion.reiniciar();
        }
    }
}

} // namespace cliente
