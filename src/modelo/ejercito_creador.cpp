#include "modelo/ejercito_creador.h"

#include <iostream>
#include <string>
#include <fstream> 
#include <unordered_map>

#include "libs/json.hpp"
#include "modelo/unidad.h"
#include "terreno.h"

namespace modelo {

EjercitoCreador::EjercitoCreador(){
}
void EjercitoCreador::inicializar(Terreno* terreno_,const nlohmann::json& ejercito){
    terreno = terreno_;
    using nlohmann::json;
    armamento.inicializar(ejercito.at("armas"));

    const json& unidades = ejercito.at("unidades");

    auto it = unidades.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for(; it != unidades.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);
        std::vector<Arma*> armas_unidad;
        for (const std::string& id_arma : elem.at("id_arma")) {
            if(!armamento.tiene(id_arma)) {
                std::cout << "Arma no encontrada: " << id_arma << std::endl;
                continue;
            }
            armas_unidad.push_back(&armamento.get(id_arma));
        }
        
        prototipos_base.emplace(elem["id"], UnidadBase(elem,armas_unidad));
    }
}
EjercitoCreador::~EjercitoCreador(){}
Unidad EjercitoCreador::clonar(std::string id_tipo,int id,int x,int y){
    std::cout << "entreo al clonar "<<std::endl;
    return Unidad(id,x,y,prototipos_base.at(id_tipo));
}
Cosechadora EjercitoCreador::clonar(std::string id_tipo,int id,int x,int y,
                                    Terreno* terreno_,int id_propietario,
                                    Jugador* comunicacion_jugador){
    return Cosechadora(id,x,y,prototipos_base.at(id_tipo),terreno_,
                                id_propietario,comunicacion_jugador);
}
unsigned int EjercitoCreador::get_costo(std::string id_tipo){
    return prototipos_base.at(id_tipo).get_costo();
}
unsigned int EjercitoCreador::get_vida(std::string id_tipo){
    return prototipos_base.at(id_tipo).get_vida();
}
std::pair<int,int>& EjercitoCreador::get_dimensiones(std::string id_tipo){
    return prototipos_base.at(id_tipo).get_dimensiones();
}
unsigned int EjercitoCreador::get_tiempo(std::string id_tipo){
    return prototipos_base.at(id_tipo).get_tiempo();
}
void EjercitoCreador::get_tiempos_de_entrenamiento(std::unordered_map<std::string,int>& tiempos){
    for (auto it=prototipos_base.begin();it!=prototipos_base.end();++it){
        tiempos.insert(std::pair<std::string,int>(it->first,(int)(it->second).get_tiempo()));
    }
}
std::set<std::string>& EjercitoCreador::get_requisitos(const std::string& clase){
    return prototipos_base.at(clase).get_requisitos();
}
std::set<std::string>& EjercitoCreador::get_casas(const std::string& clase){
    return prototipos_base.at(clase).get_casas();
}
}