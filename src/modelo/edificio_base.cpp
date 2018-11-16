#include "modelo/edificio_base.h"

#include "libs/json.hpp"

#define ID "id" 
#define ENERGIA "energia"
#define COSTO "costo"
#define DIM "dimensiones"
#define X 0
#define Y 1
#define PTOS_ESTRUCUTURA "puntos_estructura"

namespace modelo {

EdificioBase::EdificioBase(){}
EdificioBase::~EdificioBase(){}
EdificioBase::EdificioBase(const nlohmann::json& data_edificio):
                                dimensiones(std::pair<int,int>
                                (data_edificio[DIM][X],
                                data_edificio[DIM][Y])){
    tipo = data_edificio[ID];
    energia = data_edificio[ENERGIA];
    costo = data_edificio[COSTO];
    puntos_de_estructura = data_edificio[PTOS_ESTRUCUTURA];
}

unsigned int EdificioBase::get_energia() const {
    return energia;
}

unsigned int EdificioBase::get_costo() const {
    return costo;
}

unsigned int EdificioBase::get_ptos_est() {
    return puntos_de_estructura;
}
std::pair<int,int>& EdificioBase::get_dimensiones(){
    return dimensiones;
}
 unsigned int EdificioBase::get_tiempo(){
     return 5000;
 }
} // namespace modelo
