#include "modelo/ejercito.h"

#include <unordered_map>
#include <string>
#include <iostream>

#include "modelo/terreno.h"
#include "modelo/unidad.h"
#include "modelo/ejercito_creador.h"
#include "modelo/arma_creador.h"
#include "modelo/edificio.h"
#include "modelo/id.h"
#include "modelo/broadcaster.h"

namespace modelo {
    
Ejercito::Ejercito(Broadcaster& broadcaster) 
: comunicacion_jugadores(broadcaster) 
{ }

void Ejercito::inicializar(Terreno* terreno_,const nlohmann::json& ejercito_){
    prototipos.inicializar(terreno_,ejercito_);
    terreno=terreno_;
}

Ejercito::~Ejercito(){
}

int Ejercito::crear(const std::string& id_tipo, int id_propietario) {
    Posicion pos(43,56);
    Posicion posicion = terreno->obtener_posicion_libre_cercana(pos);
    if (!(terreno->rango_valido_tropa(posicion.x(),posicion.y(),prototipos.get_dimensiones(id_tipo)))) return 0; //raise error
    //std::cout << "Pase los condicionales." << '\n';
    int nuevo_id = id_.nuevo_id();
    tropas.emplace(nuevo_id,prototipos.clonar(id_tipo,nuevo_id,posicion.x(),posicion.y()));
    terreno->agregar_tropa(posicion.x(),posicion.y(),prototipos.get_dimensiones(id_tipo));
    comunicacion_jugadores.broadcast([&] (IJugador* j) {
        j->crear_tropa(nuevo_id, id_tipo, posicion.px_x(), posicion.px_y(),
            prototipos.get_vida(id_tipo), id_propietario);
    });
    return nuevo_id;
}

int Ejercito::crear_cosechadora(std::string id_tipo,Posicion& pos,
        std::vector<IJugador*>& jugadores,int id_propietario){
    Posicion posicion = terreno->obtener_posicion_libre_cercana(pos);
    if (!(terreno->rango_valido_tropa(posicion.x(),posicion.y(),prototipos.get_dimensiones(id_tipo)))) return 0; //raise error
    int nuevo_id = id_.nuevo_id();
    terreno->agregar_tropa(posicion.x(),posicion.y(),prototipos.get_dimensiones(id_tipo));
    for (auto it=jugadores.begin();it != jugadores.end();++it){
        (*it)->crear_tropa(nuevo_id,id_tipo,posicion.x(),
                    posicion.y(),prototipos.get_vida(id_tipo),id_propietario);
        if((*it)->obtener_id()==id_propietario){
            cosechadoras.emplace(nuevo_id,prototipos.clonar(
                id_tipo,nuevo_id,posicion.x(),posicion.y(),terreno,(*it)));   
            //Posicion especia = terreno->obtener_especia_cercana(posicion);
            Posicion especia(43,67);
            mover(nuevo_id,especia.x(),especia.y(),(*it));   
        }

    }
    return nuevo_id;
}
void Ejercito::destruir(int id){
    if (cosechadoras.count(id)!=0){
        terreno->eliminar_tropa(cosechadoras.at(id).get_posicion(),
                            cosechadoras.at(id).get_dimensiones());
        cosechadoras.erase(id);    
    } else{
    terreno->eliminar_tropa(tropas.at(id).get_posicion(),tropas.at(id).get_dimensiones());
    tropas.erase(id);
    }
    //crear y mandar el evento
}

void Ejercito::mover(int id, int x, int y, IJugador*) {
    std::vector<Posicion> a_estrella = 
        terreno->buscar_camino_minimo(tropas.at(id).get_posicion(), 
            Posicion(x,y));
    
    tropas.at(id).configurar_camino(a_estrella);
    
    tropas_en_movimiento.push_back(id);
    
    std::vector<std::pair<int,int>> v;
    
    for (auto it = a_estrella.begin(); it!= a_estrella.end();++it) {
        // Convertir a píxeles
        v.emplace_back(std::pair<int,int>((*it).px_x(), (*it).px_y()));
    }
    
    comunicacion_jugadores.broadcast([&] (IJugador *j) {
        j->mover_tropa(id,v);
    });
}

void Ejercito::mover_cosechadora(int id,int x,int y,IJugador* jugador){
    std::vector<Posicion> a_estrella = terreno->buscar_camino_minimo(cosechadoras.at(id).get_posicion(), 
                                                           Posicion(x,y));
    cosechadoras.at(id).configurar_camino(a_estrella);
    tropas_en_movimiento.push_back(id);
    std::vector<std::pair<int,int>> v;
    for (auto it = a_estrella.begin(); it!= a_estrella.end();++it){
        v.emplace_back(std::pair<int,int>((*it).x(),(*it).y()));
    }
    jugador->mover_tropa(id,v);
}
void Ejercito::atacar(int id_victima,int id_atacante){
    //ver si hay que seguirlo o no
    if (cosechadoras.count(id_atacante)!=0){
        //aca hay un bug
        cosechadoras.at(id_atacante).configurar_ataque(&tropas.at(id_victima));
    }else {
        //tropas.at(id_atacante).atacar(tropas.at(id_victima));
        tropas.at(id_atacante).configurar_ataque(&tropas.at(id_victima));
        tropas_atacando.push_back(id_atacante);
    }
}
void Ejercito::atacar(Edificio& edificio,int id_atacante){
    // acercarme hasta cumplir con el rango 
    if (cosechadoras.count(id_atacante)!=0){
        cosechadoras.at(id_atacante).atacar(edificio);
    }else{
        tropas.at(id_atacante).atacar(edificio);
    }
}

Unidad& Ejercito::get(int id) {
    if (cosechadoras.count(id)!=0){
        return cosechadoras.at(id);
    } else{
        return tropas.at(id);
    }
}

unsigned int Ejercito::get_costo(std::string id_tipo) {
    return prototipos.get_costo(id_tipo);
}

void Ejercito::actualizar_tropas(int dt) {
    for (std::vector<int>::iterator it = tropas_en_movimiento.begin();
            it != tropas_en_movimiento.end();)
    {
        Unidad& unidad = tropas.at(*it);
        if (!unidad.en_movimiento()){
            it = tropas_en_movimiento.erase(it);
            continue;
        }
        /*
        if (cosechadoras.count(*it)!=0){
            if (cosechadoras.at(*it)).camino_a_especia()){
                Posicion pos = terreno.obtener_refineria_cercana();
                mover_cosechadora(*it,pos.x(),pos.y(),cosechadoras.at(*it)).obtener_jugador());
            } else{
                Posicion pos = terreno.obtener_especia_cercana();
                mover_cosechadora(*it,pos.x(),pos.y(),cosechadoras.at(*it)).obtener_jugador());
            }
            cosechadoras.at(*it).actualizar_posicion(dt,terreno,jugadores);
            continue;        
        }
        */
        
        if (unidad.actualizar_posicion(dt,terreno)) {
            comunicacion_jugadores.broadcast([this, &unidad] (IJugador *j) {
                j->sincronizar_tropa(unidad.get_id(), 
                    unidad.get_posicion().px_x(),
                    unidad.get_posicion().px_y()
                );
            });
        }
        ++it;
    }
    //********************************ataque************************
    for (std::vector<int>::iterator it = tropas_atacando.begin();
            it != tropas_atacando.end();)
    {
        bool borrado = false;
        Unidad& unidad = tropas.at(*it);//ojo:se podria morir, verficar que este en el un_map
        if (!unidad.esta_atacando()){
            it = tropas_atacando.erase(it);
            continue;
        }

        int vida_nueva_victima = unidad.actualizar_ataque(dt,terreno);
        int id_victima = unidad.id_victima();
        // comunicacion_jugadores.broadcast([&] (IJugador* j) {
        //     j->atacar_tropa(id_victima, tropas.at(id_victima).get_vida());
        // });
        if (vida_nueva_victima <= 0){
            // //falta ver el tema del rango
            // for (std::vector<int>::iterator it2 = tropas_atacando.begin();
            //     it2 != tropas_atacando.end();)
            // {
            //     Unidad& atacante = tropas.at(*it2);
            //     if(atacante.id_victima() == id_victima){
            //         atacante.parar_ataque();
            //     }
            //     if(*it == id_victima){
            //         it = tropas_atacando.erase(it);//esto rompe???
            //     } else {
            //         ++it;
            //     }
            // }
            //comunicacion_jugadores.broadcast()//destruir_tropa(id_victima)
            it = tropas_atacando.erase(it);
            borrado = true;
            comunicacion_jugadores.broadcast([&] (IJugador *j) {
               j->destruir_tropa(id_victima);
            });
            //eliminar tropa muerta del modelo y comunicar
            //tropas.erase(id_victima);
            //como le aviso a jugador(clase Jugador)??
        }
        comunicacion_jugadores.broadcast([&] (IJugador *j) {
               j->atacar_tropa(id_victima,vida_nueva_victima);
        });
        //comunicacion_jugadores.broadcast()//atacar_tropa(id_victima,vida_nueva_victima)
        if (!borrado)
            ++it;
    }
}
unsigned int Ejercito::get_tiempo(std::string id_tipo){
    return prototipos.get_tiempo(id_tipo);
}
}