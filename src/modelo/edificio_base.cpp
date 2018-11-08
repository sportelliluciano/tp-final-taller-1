#include "modelo/edificio_base.h"

#include "libs/json.hpp"

#define ID "id" 
#define ENERGIA "energia"
#define COSTO "costo"
#define DIM "dimensiones"
#define X "x"
#define Y "y"
#define PTOS_ESTRUCUTURA "puntos_estructura"

namespace modelo {

EdificioBase::EdificioBase(){}
EdificioBase::~EdificioBase(){}
EdificioBase::EdificioBase(const nlohmann::json& data_edificio){
    tipo = data_edificio[ID];
    energia = data_edificio[ENERGIA];
    costo = data_edificio[COSTO];
    dimension_x = data_edificio[DIM][X];
    dimension_y = data_edificio[DIM][Y];
    puntos_de_estructura = data_edificio[PTOS_ESTRUCUTURA];
}

unsigned int EdificioBase::get_energia() const {
    return energia;
}

unsigned int EdificioBase::get_costo() const {
    return costo;
}

unsigned int EdificioBase::get_ptos_est() const {
    return puntos_de_estructura;
}

} // namespace modelo
