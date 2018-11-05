#ifndef _UNIDAD_BASE_H_
#define _UNIDAD_BASE_H_

#include <string>

#include "libs/json.hpp"

namespace modelo {

class UnidadBase {
private: 
    std::string casa;
    unsigned int rango;
    unsigned int velocidad;
    unsigned int tiempo_de_entrenamiento;
    unsigned int costo;
    unsigned int vida;

public:
    UnidadBase(const nlohmann::json& data_unidad);
    ~UnidadBase();
    unsigned int get_rango() const;
    unsigned int get_velocidad() const;
    unsigned int get_tiempo() const;
    unsigned int get_costo() const;
    unsigned int get_vida() const;
};

} // namespace modelo

#endif // _UNIDAD_BASE_H_