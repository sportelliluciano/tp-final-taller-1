#ifndef _INFRAESTRUCTURA_H_
#define _INFRAESTRUCTURA_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "modelo/edificio.h"
#include "modelo/infraestructura_creador.h"
#include "modelo/terreno.h"
#include "modelo/id.h"
#include "conexion/i_jugador.h"

#include "modelo/broadcaster.h"

namespace modelo {

class Infraestructura{
    private:
    Broadcaster& comunicacion_jugadores;
    Terreno* terreno;
    std::unordered_map<int,Edificio> edificios;
    InfraestructuraCreador prototipos;
    Id id_;

    public:
    Infraestructura(Broadcaster& broadcaster);
    void inicializar(Terreno* mapa,const nlohmann::json& edificios);
    int crear(const std::string& id_tipo,int x,int y, int id_propietario);
    unsigned int reciclar(int id);
    void destruir(int id);
    Edificio& get(int id);
    unsigned int get_costo(std::string id_tipo);
    unsigned int get_energia(std::string id_tipo);
    unsigned int get_costo(int id);
    unsigned int get_energia(int id);
    unsigned int get_tiempo(int id);
    Posicion& get_posicion(const std::string& clase);

    void actualizar_edificios(int dt_ms);

};
}
#endif
