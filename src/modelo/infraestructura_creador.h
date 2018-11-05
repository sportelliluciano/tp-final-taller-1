#ifndef _INFRAESTRUCTURA_CREADOR_H_
#define _INFRAESTRUCTURA_CREADOR_H_

#include <unordered_map>
#include <string>

#include "modelo/edificio.h"
#include "modelo/edificio_base.h"

namespace modelo {

class InfraestructuraCreador{
    private:
    std::unordered_map<char,Edificio> prototipos;
    std::unordered_map<char,EdificioBase> info_base;    
    public:

    InfraestructuraCreador(std::string path);
    ~InfraestructuraCreador();
    Edificio clonar(char id_tipo,int x,int y);
    unsigned int get_costo(char id_tipo);
};
}
#endif
