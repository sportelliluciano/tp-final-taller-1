#ifndef _EDIFICIOBASE_H_
#define _EDIFICIOBASE_H_

#include "libs/json.hpp"

namespace modelo {

class EdificioBase{
    private:
    char tipo;
    unsigned int energia;
    unsigned int costo;
    unsigned int dimension_x;
    unsigned int dimension_y;
    unsigned int puntos_de_estructura;
    
    public:
    EdificioBase(const nlohmann::json& data_edificio);
    ~EdificioBase()
    const unsigned int get_energia();
    const unsigned int get_costo();
    const unsigned int get_ptos_est();
};
}
#endif
