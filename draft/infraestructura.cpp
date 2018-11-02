#include "infraestructura.h"

#include <unordered_map>

#include "edificio.h"
#include "infraestructura_creador.h"
#include "terreno.h"

Infraestructura::Infraestructura(Terreno& terreno):terreno(terreno){}
void Infraestructura::crear_edificio(char id_tipo,int x,int y){
    Edificio nuevo_edificio = prototipos.clonar(id_tipo,x,y);
    edificios[nuevo_edificio.get_id()]=nuevo_edificio;//se podria aplicar move semantic
}
Edificio& Infraestructura::get_edificio(char id);
    return edificios.at(id);
