#include "modelo/jugador.h"

#include <string>
#include <unordered_map>
#include <set>

namespace modelo {

Jugador::Jugador(std::string casa_):casa(casa_){
}
Jugador::~Jugador(){
}
void Jugador::aumentar_consumo(unsigned int consumo_){
    consumo += consumo_;
}
void Jugador::reducir_consumo(unsigned int consumo_){
    consumo -= consumo_;
}
std::string Jugador::get_casa(){
    return casa;
}
bool Jugador::hay_suficiente_energia(unsigned int costo){
    return energia >= costo;
}
void Jugador::agregar_elemento(int id,unsigned int costo,unsigned int energia_){
    inventario.insert(id);
    energia -= costo;
    consumo += energia_;
}
void Jugador::eliminar_elemento(int id,unsigned int energia_consumida){
    consumo -= energia_consumida;
    inventario.erase(id);
}
bool Jugador::pertenece(int id){
    return inventario.count(id)>0;
}
}