#ifndef _INFRAESTRUCTURACREADOR_H_
#define _INFRAESTRUCTURACREADOR_H_

#include <unordered_map>

#include "edificio.h"

namespace modelo {

class InfraestructuraCreador{
    private:
    std::unordered_map<char,Edificio> prototipos;

    public:
    Edificio clonar(char id_tipo,int x,int y);
};
}
#endif