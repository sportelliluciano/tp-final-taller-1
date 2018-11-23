#include "infraestructura.h"

#include <unordered_map>
#include <string>

#include "modelo/edificio.h"
#include "modelo/infraestructura_creador.h"
#include "modelo/terreno.h"
#include "modelo/id.h"
#include "modelo/broadcaster.h"

#define FACTOR 0.3 //porcentaje de recuperacion al reciclar

namespace modelo {

Infraestructura::Infraestructura(Broadcaster& broadcaster,Id& id) 
: comunicacion_jugadores(broadcaster),id_(id) 
{

}

void Infraestructura::inicializar(Terreno* mapa,const nlohmann::json& edificios_){
    terreno = mapa;
    prototipos.inicializar(edificios_);
}

void Infraestructura::actualizar_edificios(int dt_ms) {

}

int Infraestructura::crear(const std::string& id_tipo, int x, int y, 
    int id_propietario) 
{
    //if (!terreno->rango_valido_edificio(x,y,prototipos.get_dimensiones(id_tipo)))return 0;//raise error;
    int nuevo_id = id_.nuevo_id();                                           
    edificios.emplace(nuevo_id,prototipos.clonar(id_tipo,nuevo_id,x,y));
    terreno->agregar_edificio(x,y,prototipos.get_dimensiones(id_tipo));
    comunicacion_jugadores.broadcast([&] (IJugador* j) {
        j->crear_edificio(
            nuevo_id,
            id_tipo,
            x, y,
            id_propietario
        );
    });
    return nuevo_id;
}
unsigned int Infraestructura::reciclar(int id){
    //revisar
    unsigned int energia_retorno = (edificios.at(id).get_costo())*FACTOR;
    destruir(id);
    return energia_retorno;
}

void Infraestructura::destruir(int id){
    terreno->eliminar_edificio(edificios.at(id).get_posicion(),edificios.at(id).get_dimensiones());
    edificios.erase (id);
    comunicacion_jugadores.broadcast([&] (IJugador* j) {
        j->eliminar_edificio(id);
    });
}

Edificio& Infraestructura::get(int id){
    return edificios.at(id);
}

unsigned int Infraestructura::get_costo(std::string id_tipo){
    return prototipos.get_costo(id_tipo);
}

unsigned int Infraestructura::get_energia(std::string id_tipo){
    return prototipos.get_energia(id_tipo);
}

unsigned int Infraestructura::get_costo(int id){
    return edificios.at(id).get_costo();
}

unsigned int Infraestructura::get_energia(int id){
    return edificios.at(id).get_energia();
}

unsigned int Infraestructura::get_tiempo(int id){
    return edificios.at(id).get_tiempo();
}

Posicion& Infraestructura::get_posicion(const std::string& clase) {
    for (auto it = edificios.begin();it!=edificios.end();++it){
        if (it-> second.get_tipo()==clase)return it->second.get_posicion();
    }
}
bool Infraestructura::pertenece(int id){
    return edificios.count(id)!= 0;
}
}
