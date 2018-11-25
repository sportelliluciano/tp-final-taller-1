#ifndef _EJERCITO_H_
#define _EJERCITO_H_

#include <unordered_map>
#include <string>
#include <unordered_set>

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
    Id& id_;
    std::unordered_set<int> tropas_en_movimiento;
    std::unordered_set<int> tropas_atacando;
    std::unordered_set<int> tropas_muertas;
    std::unordered_map<int,Cosechadora> cosechadoras;
    std::unordered_set<int> edificios_atacados;

    public:
    Ejercito(Broadcaster& broadcaster,Id& id);
    void inicializar(Terreno* terreno,const nlohmann::json& ejercito_);
    ~Ejercito();
    int crear(const std::string& id_tipo, int id_propietario);
    int crear_cosechadora(const std::string& id_tipo,int id_propietario);    
    void destruir(int id);
    void mover(int id,int x,int y);
    void mover_cosechadora(int id,int x,int y);
    void atacar(int id_victima,int id_atacante);
    void atacar(Atacable* edificio,int id_atacante);
    void matar_edificio(int id_edficio_victima);
    Unidad& get(int id);
    unsigned int get_costo(std::string id_tipo);
    unsigned int get_tiempo(std::string id_tipo);
    void matar_tropa(int id_victima,int id_atacante);
    std::unordered_set<int>& notificar_bajas();
    void actualizar_tropas(int dt);
    void eliminar_tropas();
};
}
#endif
