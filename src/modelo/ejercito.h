#ifndef _EJERCITO_H_
#define _EJERCITO_H_

#include <unordered_map>

#include "modelo/terreno.h"
#include "modelo/unidad.h"
#include "modelo/ejercito_creador.h"
#include "modelo/arma_creador.h"
#include "modelo/edificio.h"

namespace modelo {

class Ejercito{
    private:
    std::unordered_map<char,Unidad> tropas;
    EjercitoCreador prototipos;
    //ArmaCreador arma_creador;
    Terreno& terreno;

    public:
    Ejercito(Terreno& terreno);
    ~Ejercito();
    void crear(char id_tipo,int x,int y);
    void destruir(char id);
    void mover(char id,int x,int y);
    void atacar(char id_victima,char id_atacante);
    void atacar(Edificio& edificio,char id_atacante);
};
}
#endif
