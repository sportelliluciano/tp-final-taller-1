#ifndef _ARMA_BASE_H_
#define _ARMA_BASE_H_

#include <string>

#include "libs/json.hpp"

namespace modelo { class ArmaBase; }

#include "modelo/unidad.h"
#include "modelo/edificio.h"
#include "modelo/atacable.h"

namespace modelo {

class ArmaBase {
private:
    std::string tipo;
    unsigned int dano;
    unsigned int frecuencia;
    //bonificaciones
    
public:
    ArmaBase();
    ArmaBase(const nlohmann::json& data_arma);
    ~ArmaBase();
    int atacar_a(Atacable* victima);
    //void atacar_a(Edificio* victima);
    unsigned int obtener_frecuencia();
};

} // namespace modelo

#endif // _ARMA_BASE_H_
