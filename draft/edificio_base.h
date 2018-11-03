#ifndef _EDIFICIOBASE_H_
#define _EDIFICIOBASE_H_

#include "libs/json.hpp"

namespace modelo {

class EdificioBase{
    private:
    char tipo;
    const unsigned int energia;
    const unsigned int costo;
    const unsigned int dimension_x;
    const unsigned int dimension_y;
    const unsigned int puntos_de_estructura;
    
    public:
    EdificioBase(const nlohmann::json& data_edificio);
    ~EdificioBase()
    const unsigned int get_energia() const;
    const unsigned int get_costo() const;
    const unsigned int get_ptos_est() const;
};
}
#endif