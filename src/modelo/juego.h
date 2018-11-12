#ifndef _JUEGO_H_
#define _JUEGO_H_

#include <string>
#include <unordered_map>

#include "modelo/id.h"
#include "modelo/jugador.h"
#include "modelo/unidad.h"
#include "modelo/infraestructura.h"
#include "modelo/ejercito.h"
#include "modelo/terreno.h"

namespace modelo {

class Juego{
    private:
    Infraestructura inf;
    Ejercito ejercito;
    Terreno& terreno;
    std::unordered_map<int,Jugador> jugadores;
    Id id;

    public:
    Juego(Terreno& terreno);
    ~Juego();

    void agregar_jugador(std::string casa);
    void eliminar_jugador(int id);
    void crear_edificio(int id_jugador,std::string id_tipo,int x,int y);
    void reciclar_edificio(int id_jugador,int id);
    void destruir_edificio(int id_jugador,int id);
    Edificio& get(int id);//temporal
    void crear_unidad(int id_jugador,std::string id_tipo,int x,int y);
    void destruir_unidad(int id_jugador,int id);
    void mover_unidad(int id_jugador,int id,int x,int y);
    void atacar_unidad(int id_jugador,int id_victima,int id_atacante);
    void atacar_edificio(int id_jugador,int id_edificio,int id_atacante);
    Unidad& get_unidad(int id);
};
}
#endif
