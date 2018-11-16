#ifndef _EJERCITO_H_
#define _EJERCITO_H_

#include <unordered_map>
#include <string>

#include "modelo/terreno.h"
#include "modelo/unidad.h"
#include "modelo/ejercito_creador.h"
#include "modelo/arma_creador.h"
#include "modelo/edificio.h"
#include "modelo/id.h"
#include "modelo/i_jugador.h"

namespace modelo {

class Ejercito{
    private:
    std::unordered_map<int,Unidad> tropas;
    EjercitoCreador prototipos;
    Terreno& terreno;
    Id id_;
    std::vector<int> tropas_en_movimiento;

    public:
    Ejercito(Terreno& terreno);
    ~Ejercito();
    int crear(std::string id_tipo,int x,int y);
    void destruir(int id);
    void mover(int id,int x,int y,IJugador* jugador);
    void actualizar_pos(int id,int x,int y);
    void atacar(int id_victima,int id_atacante);
    void atacar(Edificio& edificio,int id_atacante);
    Unidad& get(int id);
    unsigned int get_costo(std::string id_tipo);
    unsigned int get_tiempo(std::string id_tipo);
    void empezar_movimiento(int id);
    void terminar_camino(int id);
    void actualizar_tropas(int dt,IJugador* jugador);
};
}
#endif
