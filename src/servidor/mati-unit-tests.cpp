#include <iostream>

#include "modelo/terreno.h"
#include "modelo/juego.h"

int main(int argc, char *argv[]) {
    
    modelo::Terreno terreno("../data/terreno.csv");
    modelo::Juego player(terreno);
    player.agregar_jugador("mati");
    //std::cout << terreno.rango_valido_tropa(0,4,1,1) << '\n';
    //void crear_unidad(int id_jugador,std::string id_tipo,int x,int y);
    player.crear_unidad(0,"tanque sonico",0,4);
    
    //void mover_unidad(int id_jugador,int id,int x,int y);
    player.mover_unidad(0,0,8,8);
    /*
    player.crear_edificio("fabrica_ligera",3,4);
    std::cout<< "Fabrica ligera" << '\n';
    std::cout<< "Antes: " << player.get(5).get_vida()<<'\n';
    //player.get(5).recibir_dano(100); 
    //std::cout << "Despues: " << player.get(5).get_vida() << '\n';
    player.crear_unidad("tanque sonico",3,5);
    std::cout<< "Tanque sonico" << '\n';
    player.crear_unidad("tanque sonico",3,5);
    std::cout<< "Antes: " << player.get_unidad(1).get_vida()<<'\n';
    //player.get_unidad(5).recibir_dano(30); 
    //std::cout << "Despues: " << player.get_unidad(5).get_vida() << '\n';
    player.atacar_unidad(1,0);
    std::cout << " Tanque despues: " << player.get_unidad(1).get_vida() << '\n';
    player.atacar_edificio(5,0);
    std::cout << "Edificio despues: " << player.get(5).get_vida() << '\n'; */
    
    //modelo::Terreno terreno(3,6);
    //modelo::Juego player(terreno);
    
    /*
    player.crear_unidad("tanque sonico",3,5);//0
    player.crear_unidad("tanque sonico",4,5);//1
    player.crear_unidad("tanque sonico",5,5);//2
    std::cout<< "Antes: " << player.get_unidad(1).get_vida()<<'\n';
    player.atacar_unidad(1,0);
    player.destruir_unidad(0);
    player.atacar_unidad(2,1);
    std::cout << " Tanque 1 despues: " << player.get_unidad(1).get_vida() << '\n';
    std::cout << " Tanque 2 despues: " << player.get_unidad(2).get_vida() << '\n';
    */
    
    return 0;
}