#ifndef _EDIFICIO_BASE_H_
#define _EDIFICIO_BASE_H_

#include "libs/json.hpp"
namespace modelo { class EdificioBase; }
namespace modelo {

class EdificioBase{
private:
    int tipo;
    unsigned int energia;
    unsigned int costo;
    unsigned int dimension_x;
    unsigned int dimension_y;
    unsigned int puntos_de_estructura;
    
public:
    EdificioBase();
    EdificioBase(const nlohmann::json& data_edificio);
    ~EdificioBase();
    unsigned int get_energia() const;
    unsigned int get_costo() const;
    unsigned int get_ptos_est() const;
};

} // namespace modelo

#endif // _EDIFICIO_BASE_H_
