#include "modelo/terreno.h"
#include "modelo/jugador.h"

int main(int argc, char *argv[]) {
    modelo::Terreno terreno(3,6);
    modelo::Jugador player(terreno); 
    return 0;
}