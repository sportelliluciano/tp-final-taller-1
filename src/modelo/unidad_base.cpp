#include "modelo/unidad_base.h"

#include <string>
#include <utility>

#include "libs/json.hpp"
#include "modelo/arma.h"

#define CASA "casa"
#define RANGO "rango"
#define VEL "velocidad"
#define TIEMPO "tiempo de entrenamiento"
#define COSTO "costo"
#define VIDA "vida"
#define ALTO "alto"
#define ANCHO "ancho"

namespace modelo {

UnidadBase::UnidadBase(const nlohmann::json& data_unidad,Arma& arma_):
                            arma(arma_)
                            //,dimensiones(std::pair<int,int>(data_unidad[ANCHO],
                            //                               data_unidad[ALTO])) 
                                                        {
    //casa = data_unidad[CASA];
    rango = data_unidad[RANGO];
    velocidad = data_unidad[VEL];
    tiempo_de_entrenamiento = data_unidad[TIEMPO];
    costo = data_unidad[COSTO];
    vida = data_unidad[VIDA];
}
UnidadBase::~UnidadBase() { 

}
void UnidadBase::atacar_a(Unidad* victima){
    arma.atacar_a(victima);
}
void UnidadBase::atacar_a(Edificio* victima){
    arma.atacar_a(victima);
}
unsigned int UnidadBase::get_rango() const {
    return rango;
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

} // namespace modelo
