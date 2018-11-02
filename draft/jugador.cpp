#include "jugador.h"
#include "infraestructura.h"
#include "ejercito.h"
#include "terreno.h"

    Jugador::Jugador(Terreno terreno):terreno(terreno){}
    Jugador::~Jugador(){}
    void Jugador::crear_edificio(char id_tipo,int x,int y){
        inf.crear_edificio(id_tipo,x,y);
    }
    void Jugador::crear_unidad(char id_tipo,int x,int y){
        ejercito.crear_unidad(id_tipo,x,y);
    }
    void Jugador::mover_unidad(char id,int x,int y){
        ejercito.mover_unidad(id,x,y);
    }
    void Jugador::atacar_unidad(char id_victima,char id_atacante){
        ejercito.atacar(id_victima,id_atacante);
    }
    void Jugador::atacar_edificio(char id_edificio,char id_atacante){
        ejercito.atacar(inf.get_edificio(id_edificio),id_atacante);
    }
