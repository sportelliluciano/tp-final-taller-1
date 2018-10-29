#ifndef _UNIDAD_H_
#define _UNIDAD_H_

#include "modelo/Arma.h"
#include "modelo/terreno.h"
#include "modelo/celda.h"

namespace modelo {

class Unidad{
    private:
    char casa;
    Arma& arma;
    Terreno& terreno;
    Celda posicion;
    const unsigned int rango;
    const unsigned int velocidad;
    unsigned int vida;

    public:
    Unidad(char casa,
           Arma& arma,
           Terreno& terreno,
           int x,
           int y,
           unsigned int rango,
           unsigned int velocidad,
           unsigned int vida);
    void moverse(int x, int y);
    void actualizar_pos(Posicion& pos);
    void recibir_dano(unsigned int dano);
    void atacar(char id_soldado_tipo, Unidad& victima);
    bool esta_vivo();
    void destruir();

};
}
#endif