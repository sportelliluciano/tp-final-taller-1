#include "modelo/Unidad.h"
#include "modelo/Arma.h"
#include "modelo/terreno.h"
namespace modelo {
Unidad::Unidad(char casa,
              Arma& arma,
              Terreno& terreno,
              int x,
              int y,
              unsigned int rango,
              unsigned int velocidad,
              unsigned int vida):
              casa(casa),
              arma(arma),
              terreno(terreno),
              posicion(terreno.obtener_celda(x,y)),
              rango(rango),
              velocidad(velocidad),
              vida(vida){
}
void Unidad::moverse(int x, int y){
    std::vector<Posicion> camino = terreno.buscar_camino_minimo(
            posicion, terreno.obtener_celda(x, y));
    unsigned int factor = velocidad; //*algo que tenga que ver con el tiempo
    for (std::vector<Posicion>::iterator it = camino.begin() ; it != camino.end(); ++it){
        actualizar_pos(*it);
        //wait(factor);
    }       
}
void Unidad::actualizar_pos(Posicion& pos){
    posicion = terreno.obtener_celda(pos.x(),pos.y());
    //enviar por la conexion
}
void Unidad::recibir_dano(unsigned int dano){
    vida -= dano;
    if (vida <= 0) destruir();
}
void Unidad::atacar(char id_soldado_tipo, Unidad& victima){
    //while (distancia(posicion,victima.posicion() <= rango) &&
            //victima.esta_vivo())
    arma.atacar_a(id_soldado_tipo, victima);
}
bool Unidad::esta_vivo(){
    return vida > 0;
}
void Unidad::destruir(){
    //enviar por la conexion
}
}