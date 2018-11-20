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
#include "conexion/i_jugador.h"
#include "modelo/cosechadora.h"
#include "modelo/broadcaster.h"

namespace modelo {

class Ejercito{
    private:
    Broadcaster& comunicacion_jugadores;
    std::unordered_map<int,Unidad> tropas;
    EjercitoCreador prototipos;
    Terreno* terreno;
    Id id_;
    std::vector<int> tropas_en_movimiento;
    std::vector<int> tropas_atacando;
    std::unordered_map<int,Cosechadora> cosechadoras;

    public:
    Ejercito(Broadcaster& broadcaster);
    void inicializar(Terreno* terreno,const nlohmann::json& ejercito_);
    ~Ejercito();
    int crear(const std::string& id_tipo, int id_propietario);
    int crear_cosechadora(std::string id_tipo,Posicion& pos,
        std::vector<IJugador*>& jugadores,int id_propietario);    
    void destruir(int id);
    void mover(int id,int x,int y,IJugador* jugador);
    void mover_cosechadora(int id,int x,int y,IJugador* jugador);
    void atacar(int id_victima,int id_atacante);
    void atacar(Edificio& edificio,int id_atacante);
    Unidad& get(int id);
    unsigned int get_costo(std::string id_tipo);
    unsigned int get_tiempo(std::string id_tipo);
    void actualizar_tropas(int dt);
};
}
#endif
