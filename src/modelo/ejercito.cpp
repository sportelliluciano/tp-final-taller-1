#include "modelo/ejercito.h"

#include <unordered_map>
#include <string>
#include <iostream>

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
int Ejercito::crear(std::string id_tipo,int x,int y){
    if (!(terreno.rango_valido_tropa(x,y,prototipos.get_dimensiones(id_tipo)))) return 0; //raise error
    int nuevo_id = id_.nuevo_id(); 
    tropas.emplace(nuevo_id,prototipos.clonar(id_tipo,nuevo_id,x,y));
    terreno.agregar_tropa(x,y,prototipos.get_dimensiones(id_tipo));
    return nuevo_id;
}
void Ejercito::destruir(int id){
    terreno.eliminar_tropa(tropas.at(id).get_posicion(),tropas.at(id).get_dimensiones());
    tropas.erase(id);
    //crear y mandar el evento
}
void Ejercito::mover(int id,int x,int y){
    //mandar el evento con el camino
    std::vector<Posicion> a_estrella = terreno.buscar_camino_minimo(tropas.at(id).get_posicion(), 
                                                           Posicion(x,y));
    std::vector<int> v;                                                       
    for (auto it = a_estrella.begin(); it!=a_estrella.end(); ++it){
        std::cout << "(" << (*it).x() << " ; " << (*it).y() << ")" << '\n';
        v.push_back((int)(*it).x());
        v.push_back((int)(*it).y()); 
    }                                                       
}
void Ejercito::actualizar_pos(int id,int x,int y){
    terreno.eliminar_tropa(tropas.at(id).get_posicion(),tropas.at(id).get_dimensiones());
    tropas.at(id).mover(x,y);
    terreno.agregar_tropa(x,y,tropas.at(id).get_dimensiones());
}
void Ejercito::atacar(int id_victima,int id_atacante){
    //ver si hay que seguirlo o no
    tropas.at(id_atacante).atacar(tropas.at(id_victima));
}
void Ejercito::atacar(Edificio& edificio,int id_atacante){
    // acercarme hasta cumplir con el rango 
    tropas.at(id_atacante).atacar(edificio);
}
Unidad& Ejercito::get(int id){
    return tropas.at(id);
}
unsigned int Ejercito::get_costo(std::string id_tipo){
    return prototipos.get_costo(id_tipo);
}
}