#include "infraestructura.h"

#include <unordered_map>

#include "edificio.h"
#include "infraestructura_creador.h"
#include "terreno.h"

Infraestructura::Infraestructura(Terreno& terreno):terreno(terreno){}
void Infraestructura::crear(char id_tipo,int x,int y,
                                          unsigned int energia){
    //chequear terreno
    //chequear distancia                                           
    if (energia > prototipos.get_costo(id_tipo)){                                          
        Edificio nuevo_edificio = prototipos.clonar(id_tipo,x,y);
        edificios[nuevo_edificio.get_id()]=nuevo_edificio;//se podria aplicar move semantic
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