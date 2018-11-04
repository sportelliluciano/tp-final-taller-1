#include "arma_base.h"

#include "libs/json.hpp"
#include "unidad.h"
#include "edificio.h"

ArmaBase::ArmaBase(const nlohmann::json& data_arma){
    tipo = data_arma[ID];
    dano = data_arma[DANO];
    frecuencia = data_arma[FREQ];;
}
void atacar_a(Unidad& victima){
    victima.recibir_dano(dano);
    //if (bonificaciones.count(victima.get_id())){
        //victima.recibir_dano(bonificaciones[victima.get_id()]);
    //}
}
void atacar_a(Edificio& victima){
    victima.recibir_dano(dano);
    //if (bonificaciones.count(victima.get_id())){
        //victima.recibir_dano(bonificaciones[victima.get_id()]);
    //}
}
