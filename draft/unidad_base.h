#ifndef _UNIDADBASE_H_
#define _UNIDADBASE_H_

#include <string>

#include "libs/json.hpp"
#include "arma.h"
#include "unidad.h"
#include "edificio.h"

namespace modelo {

class UnidadBase{
    private: 
    std::string casa;
    Arma& arma;
    unsigned int rango;
    unsigned int velocidad;
    unsigned int tiempo_de_entrenamiento;
    unsigned int costo;
    unsigned int vida;

    public:
    UnidadBase(const nlohmann::json& data_unidad,Arma& arma);
    ~UnidadBase();
    void atacar(Unidad& victima);
    void atacar(Edificio& edificio);
    unsigned int get_rango();
    unsigned int get_velocidad();
    unsigned int get_tiempo();
    unsigned int get_costo();
    unsigned int get_vida();
};
}
#endif