#ifndef _EJERCITO_CREADOR_H_
#define _EJERCITO_CREADOR_H_

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
    std::unordered_map<char,UnidadBase> prototipos_base;

    public:
    EjercitoCreador(Terreno& terreno);
    ~EjercitoCreador();
    Unidad& clonar(char id_tipo,int x,int y);
};
}
#endif
