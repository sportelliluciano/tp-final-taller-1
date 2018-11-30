#include "modelo/unidad_base.h"

#include <string>
#include <utility>

#include "libs/json.hpp"
#include "modelo/arma.h"
#include "modelo/atacable.h"

#define CASA "casa"
#define RANGO "rango"
#define VEL "velocidad"
#define TIEMPO "tiempo de entrenamiento"
#define COSTO "costo"
#define VIDA "vida"
#define ALTO "alto"
#define ANCHO "ancho"
#define ID "id"
#define REQUISITOS "requerimiento"

namespace modelo {

UnidadBase::UnidadBase(const nlohmann::json& data_unidad, const std::vector<Arma*>& armas_):
                            armas(armas_)
                            //,dimensiones(std::pair<int,int>(data_unidad[ANCHO],
                            //                               data_unidad[ALTO])) 
                                                        {
    //casa = data_unidad[CASA];
    clase = data_unidad[ID];
    rango = data_unidad[RANGO];
    velocidad = data_unidad[VEL];
    tiempo_de_entrenamiento = data_unidad[TIEMPO];
    costo = data_unidad[COSTO];
    vida = data_unidad[VIDA];
    casa = data_unidad[CASA].get<std::set<std::string>>();;
    requisitos =  data_unidad[REQUISITOS].get<std::set<std::string>>();
}
UnidadBase::~UnidadBase() { 

}
int UnidadBase::atacar_a(Atacable* victima) {
    if (armas.size() > 0)
        return armas.at(0)->atacar_a(victima);
    return 0;
}
unsigned int UnidadBase::get_rango() const {
    return rango*4;
}
unsigned int UnidadBase::get_velocidad() const {
    return velocidad;
}
unsigned int UnidadBase::get_tiempo() const {
    return tiempo_de_entrenamiento * 60000;//paso de segundos a milisengundos
}
unsigned int UnidadBase::get_costo() const {
    return costo;
}
unsigned int UnidadBase::get_vida() const {
    return vida;
}
std::pair<int,int>& UnidadBase::get_dimensiones() {
    return dimensiones;
}
std::string& UnidadBase::get_clase() {
    return clase;
}
unsigned int UnidadBase::obtener_frecuencia(){
    if (armas.size() > 0)
        return armas.at(0)->obtener_frecuencia();
    return 1;
}
std::set<std::string>& UnidadBase::get_requisitos(){
    return requisitos;
}
std::set<std::string>& UnidadBase::get_casas(){
    return casa;
}
} // namespace modelo
