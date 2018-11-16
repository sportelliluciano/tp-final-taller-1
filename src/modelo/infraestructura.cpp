#include "infraestructura.h"

#include <unordered_map>
#include <string>

#include "modelo/edificio.h"
#include "modelo/infraestructura_creador.h"
#include "modelo/terreno.h"
#include "modelo/id.h"

#define FACTOR 0.3 //porcentaje de recuperacion al reciclar

namespace modelo {

Infraestructura::Infraestructura(Terreno& mapa):terreno(mapa){}
int Infraestructura::crear(std::string id_tipo,int x,int y){
    if (!terreno.rango_valido_edificio(x,y,prototipos.get_dimensiones(id_tipo)))return 0;//raise error;
    int nuevo_id = id_.nuevo_id();                                           
    edificios.emplace(nuevo_id,prototipos.clonar(id_tipo,nuevo_id,x,y));
    terreno.agregar_edificio(x,y,prototipos.get_dimensiones(id_tipo));
    return nuevo_id;
}
unsigned int Infraestructura::reciclar(int id){
    //revisar
    unsigned int energia_retorno = (edificios.at(id).get_costo())*FACTOR;
    edificios.erase (id);
    return energia_retorno;
}
void Infraestructura::destruir(int id){
    terreno.eliminar_edificio(edificios.at(id).get_posicion(),edificios.at(id).get_dimensiones());
    edificios.erase (id);
    //crear y comunicar el evento
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
}
