#ifndef _JUGADOR_H_
#define _JUGADOR_H_

#include "infraestructura.h"
#include "ejercito.h"
#include "terreno.h"

namespace modelo {

class Jugador{
    private:
    Infraestructura inf;
    Ejercito ejercito;
    Terreno& terreno;

    public:
    Jugador(Terreno terreno);
    ~Jugador();
    void crear_edificio(char id,int x,int y);
    void crear_unidad(char id,int x,int y);
    void mover_unidad(char id,int x,int y);
    void atacar_unidad(char id_victima,char id_atacante);
    void atacar_edificio(char id_edificio,char id_atacante);
};
}
#endif