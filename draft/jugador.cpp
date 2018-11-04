#include "jugador.h"
#include "infraestructura.h"
#include "ejercito.h"
#include "terreno.h"

Jugador::Jugador(Terreno terreno):terreno(terreno){}
Jugador::~Jugador(){}
void Jugador::crear_edificio(char id_tipo,int x,int y){
    inf.crear(id_tipo,x,y,energia);
}
void Jugador::reciclar_edificio(char id){
    inf.reciclar(char id);
}
void Jugador::destruir_edificio(char id){
    inf.destruir(char id);
}
void Jugador::crear_unidad(char id_tipo,int x,int y){
    ejercito.crear_unidad(id_tipo,x,y);
}
void Jugador::destruir_unidad(char id){
    ejercito.destruir(id);
}
void Jugador::mover_unidad(char id,int x,int y){
    ejercito.mover_unidad(id,x,y);
}
void Jugador::atacar_unidad(char id_victima,char id_atacante){
    ejercito.atacar(id_victima,id_atacante);
}
void Jugador::atacar_edificio(char id_edificio,char id_atacante){
    ejercito.atacar(inf.get(id_edificio),id_atacante);
}
