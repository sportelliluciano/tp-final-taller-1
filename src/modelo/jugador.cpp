#include <string>

#include "modelo/jugador.h"
#include "modelo/infraestructura.h"
#include "modelo/ejercito.h"
#include "modelo/terreno.h"

namespace modelo {

Jugador::Jugador(Terreno terreno_):inf(Infraestructura(terreno_)),
                                   ejercito(Ejercito(terreno_)),
                                   terreno(terreno_){}
Jugador::~Jugador(){}
void Jugador::crear_edificio(std::string id_tipo,int x,int y){
    inf.crear(id_tipo,x,y,energia);
}
void Jugador::reciclar_edificio(int id){
    inf.reciclar(id);
}
void Jugador::destruir_edificio(int id){
    inf.destruir(id);
}
Edificio& Jugador::get(int id){//temporal
    return inf.get(id);
}
void Jugador::crear_unidad(std::string id_tipo,int x,int y){
    ejercito.crear(id_tipo,x,y);
}
void Jugador::destruir_unidad(int id){
    ejercito.destruir(id);
}
void Jugador::mover_unidad(int id,int x,int y){
    ejercito.mover(id,x,y);
}
void Jugador::atacar_unidad(int id_victima,int id_atacante){
    ejercito.atacar(id_victima,id_atacante);
}
void Jugador::atacar_edificio(int id_edificio,int id_atacante){
    ejercito.atacar(inf.get(id_edificio),id_atacante);
}
Unidad& Jugador::get_unidad(int id){//temporal
    return ejercito.get(id);
}
}
