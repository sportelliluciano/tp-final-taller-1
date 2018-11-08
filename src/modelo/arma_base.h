#ifndef _ARMA_BASE_H_
#define _ARMA_BASE_H_

#include "libs/json.hpp"

namespace modelo { class ArmaBase; }

#include "modelo/unidad.h"
#include "modelo/edificio.h"

namespace modelo {

class ArmaBase {
private:
    int tipo;
    unsigned int dano;
    unsigned int frecuencia;
    //bonificaciones
    
public:
    ArmaBase();
    ArmaBase(const nlohmann::json& data_arma);
    ~ArmaBase();
    void atacar_a(Unidad* victima);
    void atacar_a(Edificio* victima);
};

} // namespace modelo

#endif // _ARMA_BASE_H_
