#ifndef _ARMACREADOR_H_
#define _ARMACREADOR_H_

#include <unordered_map>

#include "arma_base.h"
#include "arma_caracteristicas.h"

namespace modelo {

class ArmaCreador{
    private:
    std::unordered_map<char,Arma> prototipos;
    std::unordered_map<char,ArmaBase> armas_base;
    std::unordered_map<char,ArmaBase> armas_caracteristicas;

    public:
    ArmaCreador();
    ~ArmaCreador();
    Arma& get_arma(char id);
};
}
#endif