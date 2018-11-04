#ifndef _ARMABASE_H_
#define _ARMABASE_H_

#include "libs/json.hpp"
#include "unidad.h"
#include "edificio.h"

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
