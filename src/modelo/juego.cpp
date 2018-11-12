#include <string>

#include "modelo/juego.h"
#include "modelo/infraestructura.h"
#include "modelo/ejercito.h"
#include "modelo/terreno.h"

namespace modelo {

Juego::Juego(Terreno& terreno_):inf(Infraestructura(terreno_)),
                                   ejercito(Ejercito(terreno_)),
                                   terreno(terreno_){}
Juego::~Juego(){}
void Juego::agregar_jugador(std::string casa){
    jugadores.emplace(id.nuevo_id(),Jugador(casa));
}
void Juego::eliminar_jugador(int id_){
    jugadores.erase(id_);
}
void Juego::crear_edificio(int id_jugador,std::string id_tipo,int x,int y){
    unsigned int costo = inf.get_costo(id_tipo);
    unsigned int consumo = inf.get_energia(id_tipo);
    if (jugadores.at(id_jugador).hay_suficiente_energia(costo)){
        jugadores.at(id_jugador).agregar_elemento(inf.crear(id_tipo,x,y),costo,consumo);
    }
}
void Juego::reciclar_edificio(int id_jugador,int id_){
    //necesito el consumo para bajarlo
    //necesito la energia que me devuelve (me la da inf.reciclar())
    //bajo el consumo
    //aumento la energia
    //borro del jugador 
    //borro de inf
    inf.reciclar(id_);
}
void Juego::destruir_edificio(int id_jugador,int id_){
    unsigned int energia_consumida = inf.get_energia(id_);
    inf.destruir(id_);
    jugadores.at(id_jugador).eliminar_elemento(id_,energia_consumida);
}
Edificio& Juego::get(int id_){//temporal
    return inf.get(id_);
}
void Juego::crear_unidad(int id_jugador,std::string id_tipo,int x,int y){
    unsigned int costo = ejercito.get_costo(id_tipo);
    if (jugadores.at(id_jugador).hay_suficiente_energia(costo)){
        jugadores.at(id_jugador).agregar_elemento(ejercito.crear(id_tipo,x,y),costo,0);
    }
}
void Juego::destruir_unidad(int id_jugador,int id_){
    ejercito.destruir(id_);
    jugadores.at(id_jugador).eliminar_elemento(id_,0);
}
void Juego::mover_unidad(int id_jugador,int id_,int x,int y){
    ejercito.mover(id_,x,y);
}
void Juego::atacar_unidad(int id_jugador,int id_victima,int id_atacante){
    ejercito.atacar(id_victima,id_atacante);
}
void Juego::atacar_edificio(int id_jugador,int id_edificio,int id_atacante){
    ejercito.atacar(inf.get(id_edificio),id_atacante);
}
Unidad& Juego::get_unidad(int id_){//temporal
    return ejercito.get(id_);
}
}
