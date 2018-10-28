#ifndef _COSECHADORA_H_
#define _COSECHADORA_H_

#include <cmath>

#include "cliente/entrada.h"
#include "cliente/ubicador.h"

#define max(x, y) (((x) > (y)) ? (x) : (y))

/* Velocidad de la cosechadora (en módulo) en pixeles por tick */
#define VELOCIDAD_COSECHADORA 0.05f
/**
 * Esta clase modela una cosechadora controlada por teclado.
 */
class Cosechadora {
public:
    Cosechadora() : pos_x(0), pos_y(0), entrada(Ubicador::obtener_entrada()) { 
    }

    void actualizar() {
        dir_x = dir_y = 0;
        if (entrada.esta_presionado(TECLA_IZQ)) 
            dir_x += -1;
        if (entrada.esta_presionado(TECLA_DER)) 
            dir_x +=  1;
        if (entrada.esta_presionado(TECLA_ARR)) 
            dir_y += -1;
        if (entrada.esta_presionado(TECLA_ABA)) 
            dir_y +=  1;

            
        float vx = VELOCIDAD_COSECHADORA * dir_x;
        float vy = VELOCIDAD_COSECHADORA * dir_y;
        
        pos_x += vx;
        pos_y += vy;
    }

    int x() const { return (int)pos_x; }
    int y() const { return (int)pos_y; }

    int direccion_x() const { return dir_x; }
    int direccion_y() const { return dir_y; }

private:
    float pos_x, pos_y;
    int dir_x, dir_y;
    Entrada& entrada;
};

#endif // _COSECHADORA_H_
