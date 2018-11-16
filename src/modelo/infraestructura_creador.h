#ifndef _INFRAESTRUCTURA_CREADOR_H_
#define _INFRAESTRUCTURA_CREADOR_H_

#include <unordered_map>
#include <string>

#include "modelo/edificio.h"
#include "modelo/edificio_base.h"

namespace modelo {

class InfraestructuraCreador{
    private:
    std::unordered_map<std::string,Edificio> prototipos;
    std::unordered_map<std::string,EdificioBase> info_base;    
    public:

    InfraestructuraCreador();
    ~InfraestructuraCreador();
    Edificio clonar(std::string id_tipo,int id,int x,int y);
    unsigned int get_costo(std::string id_tipo);
    unsigned int get_energia(std::string id_tipo);
    std::pair<int,int>& get_dimensiones(std::string id_tipo);
};
}
#endif
