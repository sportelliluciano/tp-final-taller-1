#ifndef _INFRAESTRUCTURA_H_
#define _INFRAESTRUCTURA_H_

#include <string>
#include <unordered_map>

#include "modelo/edificio.h"
#include "modelo/infraestructura_creador.h"
#include "modelo/terreno.h"

namespace modelo {

class Infraestructura{
    private:
    Terreno& terreno;
    std::unordered_map<int,Edificio> edificios;
    InfraestructuraCreador prototipos;

    public:
    Infraestructura(Terreno& mapa);
    void crear(std::string id_tipo,int x,int y,unsigned int energia);
    void reciclar(int id);
    void destruir(int id);
    Edificio& get(int id);
};
}
#endif
