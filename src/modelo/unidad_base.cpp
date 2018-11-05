#include "modelo/unidad_base.h"

#include <string>

#include "libs/json.hpp"
#include "modelo/arma.h"
#include "modelo/unidad.h"
#include "modelo/edificio.h"

#define CASA "casa"
#define RANGO "rango"
#define VEL "velocidad"
#define TIEMPO "tiempo de entrenamiento"
#define COSTO "costo"
#define VIDA "vida"

namespace modelo {

UnidadBase::UnidadBase(const nlohmann::json& data_unidad,Arma& arma):arma(arma){
    casa = data_unidad[CASA];
    rango = data_unidad[RANGO];
    velocidad = data_unidad[VEL];
    tiempo_de_entrenamiento = data_unidad[TIEMPO];
    costo = data_unidad[COSTO];
    vida = data_unidad[VIDA];
}
UnidadBase::~UnidadBase();
void UnidadBase::atacar(Unidad& victima){
    arma.atacar_a(victima);
}
void UnidadBase::atacar(Edificio& edificio){
    arma.atacar_a(victima);
}
unsigned int UnidadBase::get_rango(){
    return rango;
}
unsigned int UnidadBase::get_velocidad(){
    return velocidad;
}
unsigned int UnidadBase::get_tiempo(){
    return tiempo_de_entrenamiento;
}
unsigned int UnidadBase::get_costo(){
    return costo;
}
unsigned int UnidadBase::get_vida(){
    return vida;
}
}
