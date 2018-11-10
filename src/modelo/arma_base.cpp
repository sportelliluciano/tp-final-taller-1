#include "modelo/arma_base.h"

#include <string>

#include "libs/json.hpp"
#include "modelo/unidad.h"
#include "modelo/edificio.h"

#define ID "id"
#define DANO "dano"
#define FREQ "frecuencia"

namespace modelo {

ArmaBase::ArmaBase(const nlohmann::json& data_arma){
    tipo = data_arma[ID];
    dano = data_arma[DANO];
    frecuencia = data_arma[FREQ];
}
ArmaBase::~ArmaBase(){
}
void ArmaBase::atacar_a(Unidad* victima){
    victima->recibir_dano(dano);
    //if (bonificaciones.count(victima.get_id())){
        //victima.recibir_dano(bonificaciones[victima.get_id()]);
    //}
}
void ArmaBase::atacar_a(Edificio* victima){
    victima->recibir_dano(dano);
    //if (bonificaciones.count(victima.get_id())){
        //victima.recibir_dano(bonificaciones[victima.get_id()]);
    //}
}

} // namespace modelo
