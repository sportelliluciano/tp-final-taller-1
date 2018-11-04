#ifndef _EJERCITOCREADOR_H_
#define _EJERCITOCREADOR_H_

#include <unordered_map>

#include "libs/json.hpp"
#include "unidad.h"
#include "terreno.h"
#include "arma_creador.h"

namespace modelo {

class EjercitoCreador{
    private:
    Terreno& terreno;
    ArmaCreador armamento;
    //std::unordered_map<char,Unidad> prototipos;
    std::unordered_map<char,UnidadBase> prototipos_base;

    public:
    EjercitoCreador(Terreno& terreno);
    ~EjercitoCreador();
    Unidad& clonar(char id_tipo,int x,int y);
};
}
#endif
