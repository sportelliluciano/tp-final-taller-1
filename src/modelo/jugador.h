#ifndef _JUGADOR_H_
#define _JUGADOR_H_

#include <string>

#include "modelo/unidad.h"
#include "modelo/infraestructura.h"
#include "modelo/ejercito.h"
#include "modelo/terreno.h"

namespace modelo {

class Jugador{
    private:
    Infraestructura inf;
    Ejercito ejercito;
    Terreno& terreno;
    unsigned int energia = 100000;

    public:
    Jugador(Terreno terreno);
    ~Jugador();
    void crear_edificio(std::string id_tipo,int x,int y);
    void reciclar_edificio(int id);
    void destruir_edificio(int id);
    Edificio& get(int id);//temporal
    void crear_unidad(std::string id_tipo,int x,int y);
    void destruir_unidad(int id);
    void mover_unidad(int id,int x,int y);
    void atacar_unidad(int id_victima,int id_atacante);
    void atacar_edificio(int id_edificio,int id_atacante);
    Unidad& get_unidad(int id);
};
}
#endif
