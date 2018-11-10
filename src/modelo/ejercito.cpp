#include "modelo/ejercito.h"

#include <unordered_map>
#include <string>

#include "modelo/terreno.h"
#include "modelo/unidad.h"
#include "modelo/ejercito_creador.h"
#include "modelo/arma_creador.h"
#include "modelo/edificio.h"
#include "modelo/id.h"

namespace modelo {

Ejercito::Ejercito(Terreno& terreno_):prototipos(EjercitoCreador(terreno_)),
                                      terreno(terreno_){}
Ejercito::~Ejercito(){}
void Ejercito::crear(std::string id_tipo,int x,int y){
    int nuevo_id = id_.nuevo_id(); 
    tropas.emplace(nuevo_id,prototipos.clonar(id_tipo,nuevo_id,x,y));
}
void Ejercito::destruir(int id){
    tropas.erase(id);
    //crear y mandar el evento
}
void Ejercito::mover(int id,int x,int y){
    tropas.at(id).mover(x,y);
}
void Ejercito::atacar(int id_victima,int id_atacante){
    tropas.at(id_atacante).atacar(tropas.at(id_victima));
}
void Ejercito::atacar(Edificio& edificio,int id_atacante){
    tropas.at(id_atacante).atacar(edificio);
}
Unidad& Ejercito::get(int id){
    return tropas.at(id);
}
}