#ifndef _ARMA_CREADOR_H_
#define _ARMA_CREADOR_H_

#include <unordered_map>

#include "modelo/arma_base.h"
#include "modelo/arma_caracteristicas.h"

namespace modelo {

class ArmaCreador{
    private:
    std::unordered_map<char,Arma> prototipos;
    std::unordered_map<char,ArmaBase> armas_base;
    std::unordered_map<char,ArmaCaracteristicas>armas_caracteristicas;

    public:
    ArmaCreador();
    ~ArmaCreador();
    Arma& get_arma(char id);
};
}
#endif