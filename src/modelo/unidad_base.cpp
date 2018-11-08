#include "modelo/unidad_base.h"

#include <string>

#include "libs/json.hpp"
#include "modelo/arma.h"

#define CASA "casa"
#define RANGO "rango"
#define VEL "velocidad"
#define TIEMPO "tiempo de entrenamiento"
#define COSTO "costo"
#define VIDA "vida"

namespace modelo {

UnidadBase::UnidadBase(const nlohmann::json& data_unidad,Arma* arma_):arma(arma_) {
    casa = data_unidad[CASA];
    rango = data_unidad[RANGO];
    velocidad = data_unidad[VEL];
    tiempo_de_entrenamiento = data_unidad[TIEMPO];
    costo = data_unidad[COSTO];
    vida = data_unidad[VIDA];
}
UnidadBase::UnidadBase(){
    
}
UnidadBase::~UnidadBase() { 

}
void UnidadBase::atacar_a(Unidad* victima){
    arma->atacar_a(victima);
}
void UnidadBase::atacar_a(Edificio* victima){
    arma->atacar_a(victima);
}
unsigned int UnidadBase::get_rango() const {
    return rango;
}

unsigned int UnidadBase::get_velocidad() const {
    return velocidad;
}

unsigned int UnidadBase::get_tiempo() const {
    return tiempo_de_entrenamiento;
}

unsigned int UnidadBase::get_costo() const {
    return costo;
}

unsigned int UnidadBase::get_vida() const {
    return vida;
}

} // namespace modelo
