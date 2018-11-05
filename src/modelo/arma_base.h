#ifndef _ARMA_BASE_H_
#define _ARMA_BASE_H_

#include "libs/json.hpp"
#include "modelo/unidad.h"
#include "modelo/edificio.h"

namespace modelo {

class ArmaBase{
    private:
    char tipo;
    unsigned int dano;
    unsigned int frecuencia;
    //bonificaciones
    
    public:
    ArmaBase(const nlohmann::json& data_arma);
    ~ArmaBase()
    void atacar_a(Unidad& victima);
    void atacar_a(Edificio& victima);
};
}
#endif
