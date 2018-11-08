#ifndef _ARMA_CREADOR_H_
#define _ARMA_CREADOR_H_

#include <unordered_map>

#include "modelo/arma_base.h"
#include "modelo/arma_caracteristicas.h"

namespace modelo {

class ArmaCreador{
    private:
    std::unordered_map<int,Arma> prototipos;
    std::unordered_map<int,ArmaBase> armas_base;
    std::unordered_map<int,ArmaCaracteristicas>armas_caracteristicas;
    void armas_base_iniziaizador();

    public:
    ArmaCreador();
    ~ArmaCreador();
    Arma* get(int id);
};
}
#endif