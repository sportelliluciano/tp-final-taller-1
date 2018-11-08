#ifndef _INFRAESTRUCTURA_CREADOR_H_
#define _INFRAESTRUCTURA_CREADOR_H_

#include <unordered_map>
#include <string>

#include "modelo/edificio.h"
#include "modelo/edificio_base.h"

namespace modelo {

class InfraestructuraCreador{
    private:
    std::unordered_map<int,Edificio> prototipos;
    std::unordered_map<int,EdificioBase> info_base;    
    public:

    InfraestructuraCreador();
    ~InfraestructuraCreador();
    Edificio clonar(int id_tipo,int x,int y);
    unsigned int get_costo(int id_tipo);
};
}
#endif
