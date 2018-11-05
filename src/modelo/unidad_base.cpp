#include "modelo/unidad_base.h"

#include <string>

#include "libs/json.hpp"

#define CASA "casa"
#define RANGO "rango"
#define VEL "velocidad"
#define TIEMPO "tiempo de entrenamiento"
#define COSTO "costo"
#define VIDA "vida"

namespace modelo {

UnidadBase::UnidadBase(const nlohmann::json& data_unidad) {
    casa = data_unidad[CASA];
    rango = data_unidad[RANGO];
    velocidad = data_unidad[VEL];
    tiempo_de_entrenamiento = data_unidad[TIEMPO];
    costo = data_unidad[COSTO];
    vida = data_unidad[VIDA];
}

UnidadBase::~UnidadBase() { 

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
