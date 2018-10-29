#include "infanteria_ligera.h"
#define ID_INFANTERIA_LIGERA 1
using modelo::InfanteriaLigera;

InfanteriaLigera::InfanteriaLigera(char casa, char rango, int tiempo_creacion, 
        int velocidad_movimento, int id_actor_requerido, int vida, 
        int costo, int pos_x, int pos_y) : ActorArmado(casa, rango, tiempo_creacion, 
        velocidad_movimento, id_actor_requerido, vida, costo, pos_x, pos_y) {}

int InfanteriaLigera::get_id() {
    return ID_INFANTERIA_LIGERA;
}

InfanteriaLigera::~InfanteriaLigera() {}