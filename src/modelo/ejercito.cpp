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
    
Ejercito::Ejercito(Broadcaster& broadcaster,Id& id) 
: comunicacion_jugadores(broadcaster),id_(id) 
{ }

void Ejercito::inicializar(Terreno* terreno_,const nlohmann::json& ejercito_){
    prototipos.inicializar(terreno_,ejercito_);
    terreno=terreno_;
}

Ejercito::~Ejercito(){
}

int Ejercito::crear(const std::string& id_tipo, int id_propietario) {
    Posicion pos(43,56);
    Posicion posicion = terreno->obtener_posicion_caminable_cercana(pos);
    if (!(terreno->rango_valido_tropa(posicion.x(),posicion.y(),prototipos.get_dimensiones(id_tipo)))) return 0; //raise error
    int nuevo_id = id_.nuevo_id();
    tropas.emplace(nuevo_id,prototipos.clonar(id_tipo,nuevo_id,posicion.x(),posicion.y()));
    terreno->agregar_tropa(posicion, prototipos.get_dimensiones(id_tipo));
    comunicacion_jugadores.broadcast([&] (IJugador* j) {
        j->crear_tropa(nuevo_id, id_tipo, posicion.px_x(), posicion.px_y(),
            prototipos.get_vida(id_tipo), id_propietario);
    });
    return nuevo_id;
}

int Ejercito::crear_cosechadora(const std::string& id_tipo,int id_propietario){
    Posicion pos(43,56);
    Posicion posicion = terreno->obtener_posicion_caminable_cercana(pos);
    if (!(terreno->rango_valido_tropa(posicion.x(),posicion.y(),prototipos.get_dimensiones(id_tipo)))) return 0; //raise error
    int nuevo_id = id_.nuevo_id();
    terreno->agregar_tropa(posicion, prototipos.get_dimensiones(id_tipo));
    cosechadoras.emplace(nuevo_id,prototipos.clonar(
            id_tipo,nuevo_id,posicion.x(),posicion.y(),terreno,id_propietario));
    //Posicion especia = terreno->obtener_especia_cercana(posicion);
    Posicion especia(43,67);
    
    comunicacion_jugadores.broadcast([&] (IJugador* j) {
        j->crear_tropa(nuevo_id, id_tipo, posicion.px_x(), posicion.px_y(),
            prototipos.get_vida(id_tipo), id_propietario);
    });
    mover_cosechadora(nuevo_id,especia.x(),especia.y());   
    return nuevo_id;
}

void Ejercito::mover(int id, int x, int y) {
    Posicion destino = terreno->obtener_posicion_caminable_cercana(
        Posicion(x, y));
    
    std::vector<Posicion> a_estrella = 
        terreno->buscar_camino_minimo(tropas.at(id).get_posicion(), destino);
    
    tropas.at(id).configurar_camino(a_estrella);
    
    tropas_en_movimiento.insert(id);
    
    std::vector<std::pair<int,int>> v;
    
    for (auto it = a_estrella.begin(); it!= a_estrella.end();++it) {
        v.emplace_back(std::pair<int,int>((*it).px_x(), (*it).px_y()));
    }
    
    comunicacion_jugadores.broadcast([&] (IJugador *j) {
        j->mover_tropa(id,v);
    });
}

void Ejercito::mover_cosechadora(int id,int x,int y){
    Cosechadora& cosechadora = cosechadoras.at(id);
    std::vector<Posicion> a_estrella = terreno->buscar_camino_minimo(cosechadora.get_posicion(), 
                                                           Posicion(x,y)); 
    cosechadora.configurar_camino(a_estrella);
    tropas_en_movimiento.insert(id);
    std::vector<std::pair<int,int>> v;
    for (auto it = a_estrella.begin(); it!= a_estrella.end();++it){
        v.emplace_back(std::pair<int,int>((*it).px_x(),(*it).px_y()));
    }
    comunicacion_jugadores.broadcast([&] (IJugador *j) {
        j->mover_tropa(id,v);
    });
}
void Ejercito::atacar(int id_victima,int id_atacante){
    //ver si hay que seguirlo o no
    if (tropas.count(id_atacante)!=0 && tropas.count(id_victima)!=0 ){
        tropas.at(id_atacante).configurar_ataque(&tropas.at(id_victima));
        tropas_atacando.insert(id_atacante);
    }
    if (tropas.count(id_atacante)!=0 && cosechadoras.count(id_victima)!=0 ){
        tropas.at(id_atacante).configurar_ataque(&cosechadoras.at(id_victima));
        tropas_atacando.insert(id_atacante);
    }

}
void Ejercito::atacar(Atacable* edificio,int id_atacante){
    // acercarme hasta cumplir con el rango 
    if (tropas.count(id_atacante)!=0) {
        tropas.at(id_atacante).configurar_ataque(edificio);
        tropas_atacando.insert(id_atacante);
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

void Ejercito::matar_tropa(int id_victima,int id_atacante){
    tropas_muertas.insert(id_victima);
    tropas.at(id_atacante).parar_ataque();
    if (tropas_en_movimiento.count(id_victima)!=0){
        tropas_en_movimiento.erase(id_victima);
    }
    if (tropas.count(id_victima)==0 && cosechadoras.count(id_victima)==0){
        //es edificio
        return;
    }
    comunicacion_jugadores.broadcast([&] (IJugador *j) {
               j->destruir_tropa(id_victima);
            });
}
void Ejercito::eliminar_tropas(){
    for (auto it = tropas_muertas.begin();
            it != tropas_muertas.end();++it)
    {
        if (tropas.count(*it != 0)){
            terreno->eliminar_tropa(tropas.at(*it).get_posicion(),
                                    tropas.at(*it).get_dimensiones());
            tropas.erase(*it);
        } else if (cosechadoras.count(*it)!=0){
            terreno->eliminar_tropa(cosechadoras.at(*it).get_posicion(),
                            cosechadoras.at(*it).get_dimensiones());
            cosechadoras.erase(*it);
        } else{
            return;//es un edificio
        }
    }
}

std::unordered_set<int>& Ejercito::notificar_bajas(){
    return tropas_muertas;
}
 /*
        si es una cosechadora
            si no llego a destino
                actualizar posicion
            si llego(cuando llego pongo descando en true)
                si estas operando
                    pasar tiempo
                si no esta operando (ya opero)
                    si esta en camino a la especia(cuadno llego a la epecia lo pongo en false)
                        nueva posicion = posicion especia
                    sino
                        nueva psocion = posicion refineria
                    mover a neuva posciom      
        */
void Ejercito::actualizar_cosechadoras(int dt,Cosechadora& cosechadora){
    if (cosechadora.operando()){
            cosechadora.operar(dt);
    } else if (!cosechadora.en_movimiento()){
         //try por si no hay posicion (no hay refinerias o no hay mas especia)
        Posicion posicion;
        if (cosechadora.camino_a_especia()){
            std::cout << "voy a la especia"<< std::endl;
            posicion = terreno->obtener_especia_cercana(cosechadora.get_posicion());
            std::cout << "En posicion: "<<posicion.x() <<" - "<< posicion.y() <<std::endl;
            posicion = Posicion(posicion.x(),posicion.y());
            //posicion = Posicion(43,67); //especia
        } else {
            posicion = terreno->obtener_refinerias_cercana(
                                            cosechadora.get_posicion(),
                                            cosechadora.obtener_id_jugador());
        }
        //catch
            //return
        posicion = terreno->obtener_posicion_caminable_cercana(posicion);    
        mover_cosechadora(cosechadora.get_id(),posicion.x(),posicion.y());
        std::cout << "en camino"<< std::endl;
    } else{
        if (cosechadora.actualizar_posicion(dt,terreno)) {
            comunicacion_jugadores.broadcast([this, &cosechadora] (IJugador *j) {
            j->sincronizar_tropa(cosechadora.get_id(), 
                cosechadora.get_posicion().px_x(),
                cosechadora.get_posicion().px_y()
                );
            });
        }        
    }
}

void Ejercito::actualizar_movimiento(int dt){
    for (auto it = tropas_en_movimiento.begin();
            it != tropas_en_movimiento.end();)
    {
        if (cosechadoras.count(*it)!=0){
            Cosechadora& cosechadora = cosechadoras.at(*it);
            actualizar_cosechadoras(dt,cosechadora);
            ++it;
            continue;
        }
        Unidad& unidad = tropas.at(*it);
        if (!unidad.en_movimiento()){
            it = tropas_en_movimiento.erase(it);
            continue;
        }
        
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
}

void Ejercito::limpiar_tropas_atacando(){
    //saco a los muertos restantes del set
    for (auto it = tropas_atacando.begin();
            it != tropas_atacando.end();)
    {
        if (tropas_muertas.count(*it)!= 0){
            //aquellos que atacaron y despues murieron
            it = tropas_atacando.erase(it);
            continue;
        } else if (tropas_muertas.count(tropas.at(*it).id_victima())!=0){
            //aquellos que atacaron y no mataron a su victima pero despues alguien mas la mato
            tropas.at(*it).parar_ataque();
            it = tropas_atacando.erase(it);
            continue;
        }
        ++it;
    }
}

void Ejercito::actualizar_ataques(int dt){
     tropas_muertas.clear();
    for (auto it = tropas_atacando.begin();
            it != tropas_atacando.end();)
    {
        bool borrado = false;
        if (tropas_muertas.count(*it)!= 0) {
            it = tropas_atacando.erase(it);
            continue;
        }
        // el verificar la muerte aca hace que limpiar_tropas_atacado sea mas rapido 
        Unidad& unidad = tropas.at(*it);
        if (!unidad.esta_atacando()) {
            it = tropas_atacando.erase(it);
            continue;
        }
        int id_victima = unidad.id_victima();
        if (tropas_muertas.count(id_victima) != 0){
            tropas.at(*it).parar_ataque();
            it = tropas_atacando.erase(it);
            continue;
        }
        //falta ver el tema del rango
        int vida_nueva_victima = unidad.actualizar_ataque(dt,terreno);
        if (vida_nueva_victima <= 0) {
            matar_tropa(id_victima,*it);//lo saca del modelo
            it = tropas_atacando.erase(it);
            borrado = true;
        } else {
            comunicacion_jugadores.broadcast([&] (IJugador *j) {
                j->atacar(unidad.get_id(), id_victima, vida_nueva_victima);
            });
        }
        if (!borrado)
            ++it;
    }
    //saco a los muertos restantes del set
    limpiar_tropas_atacando();
    eliminar_tropas();
}

void Ejercito::actualizar_tropas(int dt) {
    actualizar_movimiento(dt);
    actualizar_ataques(dt);
}
unsigned int Ejercito::get_tiempo(std::string id_tipo){
    return prototipos.get_tiempo(id_tipo);
}
}