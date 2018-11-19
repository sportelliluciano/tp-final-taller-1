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
    void armas_base_iniziaizador();

    public:
    ArmaCreador();
    void inicializar();
    ~ArmaCreador();
    Arma& get(std::string id);
    bool tiene(std::string id);
};
}
#endif