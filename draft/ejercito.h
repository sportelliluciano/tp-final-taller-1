#ifndef _EJERCITO_H_
#define _EJERCITO_H_

#include <unordered_map>

#include "terreno.h"
#include "unidad.h"

namespace modelo {

class Ejercito{
    private:
    std::unordered_map<char,Unidad> tropas;
    Terreno& terreno;

    public:
    Ejercito(Terreno terreno);
    ~Ejercito();
    void crear_unidad(char id_tipo,int x,int y);
    void mover_unidad(char id,int x,int y);
    void atacar(char id_victima,char id_atacante);
    void atacar(Edificio& edificio,char id_atacante);
};
}
#endif