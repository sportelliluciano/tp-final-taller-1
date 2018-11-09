#include "infraestructura.h"

#include <unordered_map>
#include <string>

#include "modelo/edificio.h"
#include "modelo/infraestructura_creador.h"
#include "modelo/terreno.h"

namespace modelo {

Infraestructura::Infraestructura(Terreno& mapa):terreno(mapa){}
void Infraestructura::crear(std::string id_tipo,int x,int y,
                                          unsigned int energia){
    //chequear terreno
    //chequear distancia
    int nuevo_id = 5;// 5 es un ejemplo, hay que imlementar un generador de ids                                           
    if (energia > prototipos.get_costo(id_tipo)){                                          
        //Edificio nuevo_edificio = prototipos.clonar(id_tipo,x,y);
        //edificios.emplace(nuevo_edificio.get_id(),nuevo_edificio);
        edificios.emplace(nuevo_id,prototipos.clonar(id_tipo,nuevo_id,x,y));
    }
}
void Infraestructura::reciclar(int id){
     edificios.erase (id);
    //crear y comunicar el evento
}
void Infraestructura::destruir(int id){
    edificios.erase (id);
    //crear y comunicar el evento
}
Edificio& Infraestructura::get(int id){
    return edificios.at(id);
}
}
