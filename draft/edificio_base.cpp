#include "edificio_base.h"

#include "libs/json.hpp"

#define ID "id" 
#define ENERGIA "energia"
#define COSTO "costo"
#define DIM "dimensiones"
#define X "x"
#define Y "y"
#define PTOS_ESTRUCUTURA "puntos_estructura"

EdificioBase::EdificioBase(const nlohmann::json& data_edificio){
    tipo = data_edificio[ID];
    energia = data_edificio[ENERGIA];
    costo = data_edificio[COSTO];
    dimension_x = data_edificio[DIM][X];
    dimension_y = data_edificio[DIM][Y];
    puntos_de_estructura = data_edificio[PTOS_ESTRUCUTURA];
}
const unsigned int EdificioBase::get_energia(){
    return energia;
}
const unsigned int EdificioBase::get_costo(){
    return costo;
}
const unsigned int EdificioBase::get_ptos_est(){
    return puntos_de_estructura;
}