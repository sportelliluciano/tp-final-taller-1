#include "Edificio.h"
#include "modelo/terreno.h"
#include "modelo/celda.h"

Edificio::Edificio(int x, int y, Terreno& terreno, unsigned int energia,
                                    unsigned int costo,
                                    unsigned int dimensiones,
                                    unsigned int puntos_de_estructura,
                                    char tipo):
                                    terreno(terreno),
                                    posicion(terreno.obtener_celda(x,y)),
                                    energia(energia),
                                    puntos_de_estructura(puntos_de_estructura),
                                    tipo(tipo){
    //chequeo que la distancia y el tipo de suelo sea el corrcto, sino rompo
    //if (posicion.tipo() != 3) rompe //digamos 3=roca
    for (int j=0, j<dimension_y; j++){
        for (int i=0; i<dimension_x; i++){
            if (terreno.obtener_celda(x+i, y+j).tipo() != 3){
                //rompe
            }
        }
    }
    int x_i =x - 5; //cantidad de bloques de distancia
    int y_i =y - 5;
    for (int j=0, j<3*5; j++){
        for (int i=0; i<3*5; i++){
            //if (terreno.esta_ocupado(x+i, y+j) != 3){
                //rompe
            //}
        }
    }

}
void Edificio::recibir_dano(unsigned int dano){
    puntos_de_estructura -= dano;
    //if (puntos_de_estructura <= 0);//comunicar destruccion
}
void Edificio::Destruir(){
    //comunicar actualizacion(tener en cuenta dismunir el contador
    // de infraestructura y devolver energia)
}