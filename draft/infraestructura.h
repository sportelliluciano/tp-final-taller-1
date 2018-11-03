#ifndef _INFRAESTRUCTURA_H_
#define _INFRAESTRUCTURA_H_

#include <unordered_map>

#include "edificio.h"
#include "infraestructura_creador.h"
#include "terreno.h"

namespace modelo {

class Infraestructura{
    private:
    std::unordered_map<char,Edificio> edificios;
    InfraestructuraCreador prototipos;
    Terreno& terreno;

    public:
    Infraestructura(Terreno& terreno);
    void crear(char id_tipo,int x,int y,unsigned int energia);
    void reciclar(char id);
    void destruir(char id);
    Edificio& get(char id);
};
}
#endif