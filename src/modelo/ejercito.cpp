#include "modelo/ejercito.h"

#include <unordered_map>

#include "modelo/terreno.h"
#include "modelo/unidad.h"
#include "modelo/ejercito_creador.h"
#include "modelo/arma_creador.h"
#include "modelo/edificio.h"

namespace modelo {

Ejercito::Ejercito(Terreno& terreno_):prototipos(EjercitoCreador(terreno_)),
                                      terreno(terreno_){}
Ejercito::~Ejercito(){}
void Ejercito::crear(char id_tipo,int x,int y){
    Unidad nueva_unidad = prototipos.clonar(id_tipo,x,y);
     tropas.emplace(nueva_unidad.get_id(),nueva_unidad); // usar move semantics
    //tropas[nueva_unidad.get_id()] = nueva_unidad; // usar move semantics
}
void Ejercito::destruir(char id){
    tropas.erase(id);
    //crear y mandar el evento
}
void Ejercito::mover(char id,int x,int y){
    tropas[id].mover(x,y);
}
void Ejercito::atacar(char id_victima,char id_atacante){
    tropas[id_atacante].atacar(tropas[id_victima]);
}
void Ejercito::atacar(Edificio& edificio,char id_atacante){
    tropas[id_atacante].atacar(edificio);
}
}