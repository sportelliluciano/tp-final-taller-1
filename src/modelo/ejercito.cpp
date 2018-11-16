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
    std::cout << "Pase los condicionales." << '\n';
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
void Ejercito::mover(int id,int x,int y,IJugador* jugador){
    std::vector<Posicion> a_estrella = terreno.buscar_camino_minimo(tropas.at(id).get_posicion(), 
                                                           Posicion(x,y));
    tropas.at(id).configurar_camino(a_estrella);
    tropas_en_movimiento.push_back(id);
    //jugador->mover_tropa(id,a_estrella);
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
void Ejercito::terminar_camino(int id){
    for (std::vector<int>::iterator it = tropas_en_movimiento.begin();
        it != tropas_en_movimiento.end();++it){
            if ((*it) == id){
                tropas_en_movimiento.erase(it);
                break;
            }
        }
}
void Ejercito::actualizar_tropas(int dt,IJugador* jugador){
    for (std::vector<int>::iterator it = tropas_en_movimiento.begin();
            it != tropas_en_movimiento.end(); ++it){
        if (tropas.at(*it).en_movimiento()){
            it = tropas_en_movimiento.erase(it);
            continue;
        }
        tropas.at(*it).actualizar_posicion(dt,jugador);
    }
}
unsigned int Ejercito::get_tiempo(std::string id_tipo){
    return prototipos.get_tiempo(id_tipo);
}
}