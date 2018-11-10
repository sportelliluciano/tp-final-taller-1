#ifndef _ARMA_CREADOR_H_
#define _ARMA_CREADOR_H_

#include <unordered_map>
#include <string>

#include "modelo/arma_base.h"
//#include "modelo/arma_caracteristicas.h"

namespace modelo {

class ArmaCreador{
    private:
    std::unordered_map<std::string,Arma> prototipos;
    std::unordered_map<std::string,ArmaBase> armas_base;
    //std::unordered_map<std::string,ArmaCaracteristicas>armas_caracteristicas;
    void armas_base_iniziaizador();

    public:
    ArmaCreador();
    ~ArmaCreador();
    Arma& get(std::string id);
    bool tiene(std::string id);
};
}
#endif