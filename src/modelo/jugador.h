#ifndef _JUGADOR_H_
#define _JUGADOR_H_

#include "modelo/infraestructura.h"
#include "modelo/ejercito.h"
#include "modelo/terreno.h"

namespace modelo {

class Jugador{
    private:
    Infraestructura inf;
    Ejercito ejercito;
    Terreno& terreno;
    unsigned int energia;

    public:
    Jugador(Terreno terreno);
    ~Jugador();
    void crear_edificio(char id_tipo,int x,int y);
    void reciclar_edificio(char id);
    void destruir_edificio(char id);
    void crear_unidad(char id_tipo,int x,int y);
    void destruir_unidad(char id);
    void mover_unidad(char id,int x,int y);
    void atacar_unidad(char id_victima,char id_atacante);
    void atacar_edificio(char id_edificio,char id_atacante);
};
}
#endif
