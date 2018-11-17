#ifndef _EDIFICIO_BASE_H_
#define _EDIFICIO_BASE_H_

#include <string>
#include <utility>

#include "libs/json.hpp"
namespace modelo { class EdificioBase; }
namespace modelo {

class EdificioBase{
private:
    std::string tipo;
    unsigned int energia;
    unsigned int costo;
    std::pair<int,int> dimensiones;
    unsigned int puntos_de_estructura;
    
public:
    EdificioBase();
    EdificioBase(const nlohmann::json& data_edificio);
    ~EdificioBase();
    unsigned int get_energia() const;
    unsigned int get_costo() const;
    unsigned int get_ptos_est() ;
    std::pair<int,int>& get_dimensiones();
    unsigned int get_tiempo() ;
    std::string& get_tipo();
};

} // namespace modelo

#endif // _EDIFICIO_BASE_H_
