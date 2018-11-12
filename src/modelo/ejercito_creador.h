#ifndef _EJERCITO_CREADOR_H_
#define _EJERCITO_CREADOR_H_

#include <string>
#include <unordered_map>  

#include "libs/json.hpp"
#include "modelo/unidad.h"
#include "modelo/terreno.h"
#include "modelo/arma_creador.h"

namespace modelo {

class EjercitoCreador{
    private:
    Terreno& terreno;
    ArmaCreador armamento;
    //std::unordered_map<char,Unidad> prototipos;
    std::unordered_map<std::string,UnidadBase> prototipos_base;

    public:
    EjercitoCreador(Terreno& terreno_);
    ~EjercitoCreador();
    Unidad clonar(std::string id_tipo,int id,int x,int y);
    unsigned int get_costo(std::string id_tipo);
    std::pair<int,int>& get_dimensiones(std::string id_tipo);
};
}
#endif
