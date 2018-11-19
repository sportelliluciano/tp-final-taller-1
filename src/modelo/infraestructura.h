#ifndef _INFRAESTRUCTURA_H_
#define _INFRAESTRUCTURA_H_

#include <string>
#include <unordered_map>

#include "modelo/edificio.h"
#include "modelo/infraestructura_creador.h"
#include "modelo/terreno.h"
#include "modelo/id.h"

namespace modelo {

class Infraestructura{
    private:
    Terreno* terreno;
    std::unordered_map<int,Edificio> edificios;
    InfraestructuraCreador prototipos;
    Id id_;

    public:
    Infraestructura();
    void inicializar(Terreno* mapa,const nlohmann::json& edificios);
    int crear(std::string id_tipo,int x,int y);
    unsigned int reciclar(int id);
    void destruir(int id);
    Edificio& get(int id);
    unsigned int get_costo(std::string id_tipo);
    unsigned int get_energia(std::string id_tipo);
    unsigned int get_costo(int id);
    unsigned int get_energia(int id);
    unsigned int get_tiempo(int id);
    Posicion& get_posicion(const std::string& clase);
};
}
#endif
