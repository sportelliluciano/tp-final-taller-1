#include "infraestructura.h"

#include <unordered_map>

#include "modelo/edificio.h"
#include "modelo/infraestructura_creador.h"
#include "modelo/terreno.h"

namespace modelo {

Infraestructura::Infraestructura(Terreno& mapa):terreno(mapa){}
void Infraestructura::crear(char id_tipo,int x,int y,
                                          unsigned int energia){
    //chequear terreno
    //chequear distancia                                           
    if (energia > prototipos.get_costo(id_tipo)){                                          
        Edificio nuevo_edificio = prototipos.clonar(id_tipo,x,y);
        edificios.emplace(nuevo_edificio.get_id(),nuevo_edificio);
        //edificios[nuevo_edificio.get_id()]=nuevo_edificio;//se podria aplicar move semantic
    }
}
void Infraestructura::reciclar(char id){
     edificios.erase (id);
    //crear y comunicar el evento
}
void Infraestructura::destruir(char id){
    edificios.erase (id);
    //crear y comunicar el evento
}
Edificio& Infraestructura::get(char id){
    return edificios.at(id);
}
}
