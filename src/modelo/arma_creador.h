#ifndef _ARMA_CREADOR_H_
#define _ARMA_CREADOR_H_

#include <unordered_map>
#include <string>

#include "libs/json.hpp"

#include "modelo/arma_base.h"
//#include "modelo/arma_caracteristicas.h"

namespace modelo {

class ArmaCreador{
    private:
    std::unordered_map<std::string,Arma> prototipos;
    std::unordered_map<std::string,ArmaBase> armas_base;
    void inicializar_armas_base(const nlohmann::json& data_armas_base);

    public:
    ArmaCreador();
    void inicializar(const nlohmann::json& data_armas_base);
    ~ArmaCreador();
    Arma& get(std::string id);
    bool tiene(std::string id);
};
}
#endif