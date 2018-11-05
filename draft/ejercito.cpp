#include "ejercito.h"

#include <unordered_map>

#include "terreno.h"
#include "unidad.h"
#include "ejercito_creador.h"
#include "arma_creador.h"
#include "edificio.h"

Ejercito::Ejercito(Terreno& terreno):terreno(terreno){}
Ejercito::~Ejercito(){}
void Ejercito::crear(char id_tipo,int x,int y){
    Unidad nueva_unidad = prototipos.clonar(id_tipo,x,y);
    tropas[nueva_unidad.get_id()] = nueva_unidad; // usar move semantics
}
void Ejercito::destruir(char id){
    tropas.erace(id);
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
