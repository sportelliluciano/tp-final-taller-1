#ifndef _INFRAESTRUCTURACREADOR_H_
#define _INFRAESTRUCTURACREADOR_H_

#include <unordered_map>
#include <string>

#include "edificio.h"
#include "edificio_base.h"

namespace modelo {

class InfraestructuraCreador{
    private:
    std::unordered_map<char,Edificio> prototipos;
    std::unordered_map<char,EdificioBase> info_base;    
    public:

    InfraestructuraCreador(std::string path);
    ~InfraestructuraCreador();
    Edificio clonar(char id_tipo,int x,int y);
};
}
#endif