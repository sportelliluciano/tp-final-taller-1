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
    Posicion posicion = terreno->obtener_posicion_libre_cercana(pos);
    if (!(terreno->rango_valido_tropa(posicion.x(),posicion.y(),prototipos.get_dimensiones(id_tipo)))) return 0; //raise error
    //std::cout << "Pase los condicionales." << '\n';
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
    Posicion posicion = terreno->obtener_posicion_libre_cercana(pos);
    if (!(terreno->rango_valido_tropa(posicion.x(),posicion.y(),prototipos.get_dimensiones(id_tipo)))) return 0; //raise error
    int nuevo_id = id_.nuevo_id();
    std::cout << "el Id de cosechadora es  "<< nuevo_id<<std::endl;
    terreno->agregar_tropa(posicion, prototipos.get_dimensiones(id_tipo));
    cosechadoras.emplace(nuevo_id,prototipos.clonar(
            id_tipo,nuevo_id,posicion.x(),posicion.y(),terreno));
    std::cout << "salio al clonar "<<std::endl;            
    //Posicion especia = terreno->obtener_especia_cercana(posicion);
    Posicion especia(43,67);
    
    comunicacion_jugadores.broadcast([&] (IJugador* j) {
        j->crear_tropa(nuevo_id, id_tipo, posicion.px_x(), posicion.px_y(),
            prototipos.get_vida(id_tipo), id_propietario);
    });
    mover_cosechadora(nuevo_id,especia.x(),especia.y());   
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

void Ejercito::mover(int id, int x, int y) {
    std::vector<Posicion> a_estrella = 
        terreno->buscar_camino_minimo(tropas.at(id).get_posicion(), 
            Posicion(x,y));
    
    tropas.at(id).configurar_camino(a_estrella);
    
    tropas_en_movimiento.insert(id);
    
    std::vector<std::pair<int,int>> v;
    
    for (auto it = a_estrella.begin(); it!= a_estrella.end();++it) {
        // Convertir a píxeles
        v.emplace_back(std::pair<int,int>((*it).px_x(), (*it).px_y()));
    }
    
    comunicacion_jugadores.broadcast([&] (IJugador *j) {
        j->mover_tropa(id,v);
    });
}

void Ejercito::mover_cosechadora(int id,int x,int y){
    std::vector<Posicion> a_estrella = terreno->buscar_camino_minimo(cosechadoras.at(id).get_posicion(), 
                                                           Posicion(x,y));
    cosechadoras.at(id).configurar_camino(a_estrella);
    tropas_en_movimiento.insert(id);
    std::vector<std::pair<int,int>> v;
    for (auto it = a_estrella.begin(); it!= a_estrella.end();++it){
        v.emplace_back(std::pair<int,int>((*it).px_x(),(*it).px_y()));
    }
    std::cout << "entro a mover cosechadora" << std::endl;
    comunicacion_jugadores.broadcast([&] (IJugador *j) {
        j->mover_tropa(id,v);
    });
    std::cout << "salgo de mover cosechadora" << std::endl;
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
    edificios_atacados.insert(edificio->get_id());
}
void Ejercito::matar_edificio(int id_edficio_victima){
    edificios_atacados.erase(id_edficio_victima);
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
        matar_edificio(id_victima);
        return;
    }
    std::cout << "se comunico la baja" << std::endl;
    comunicacion_jugadores.broadcast([&] (IJugador *j) {
               j->destruir_tropa(id_victima);
            });
}
void Ejercito::eliminar_tropas(){
    for (auto it = tropas_muertas.begin();
            it != tropas_muertas.end();++it)
    {
        if (tropas.count(*it != 0)){
            tropas.erase(*it);
        } else if (cosechadoras.count(*it)!=0){
            cosechadoras.erase(*it);
        } else{
            std::cout << "no se comunico la baja" << std::endl;
            return;//es un edificio
        }
    }
}

std::unordered_set<int>& Ejercito::notificar_bajas(){
    return tropas_muertas;
}

void Ejercito::actualizar_tropas(int dt) {
    for (auto it = tropas_en_movimiento.begin();
            it != tropas_en_movimiento.end();)
    {
        
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
        if (cosechadoras.count(*it)!=0){
            Cosechadora& cosechadora = cosechadoras.at(*it);
            if (cosechadora.operando()){
                //std::cout << "operando"  << std::endl;
                cosechadora.operar(dt);
            } else if (!cosechadora.en_movimiento()){
                std::cout << "cambio de camino"  << std::endl;
                Posicion posicion;
                if (cosechadora.camino_a_especia()){
                    std::cout << "a especia"  << std::endl;
                    posicion = Posicion(43,67); //especia
                } else {
                    std::cout << "a refineria"  << std::endl;
                    posicion = Posicion(36,36);//refineria
                }
                std::cout << "x: "<<posicion.px_x() <<" y: "<< posicion.px_y() << std::endl;
                mover_cosechadora(*it,posicion.x(),posicion.y());
            } else{
                //std::cout << "actualizo"  << std::endl;
                if (cosechadora.actualizar_posicion(dt,terreno)) {
                    comunicacion_jugadores.broadcast([this, &cosechadora] (IJugador *j) {
                        j->sincronizar_tropa(cosechadora.get_id(), 
                            cosechadora.get_posicion().px_x(),
                            cosechadora.get_posicion().px_y()
                        );
                    });
                }        
            }
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
    //********************************ataque************************
    tropas_muertas.clear();
    for (auto it = tropas_atacando.begin();
            it != tropas_atacando.end();)
    {
        bool borrado = false;
        if (tropas_muertas.count(*it)!= 0) {
            it = tropas_atacando.erase(it);
            continue;
        }
        // el verificar la muerte aca hace que el ciclo de elimnado de muertos
        //de tropa_atacando sea mas rapido 
        Unidad& unidad = tropas.at(*it);
        if (!unidad.esta_atacando()) {
            it = tropas_atacando.erase(it);
            continue;
        }
        //std::cout << "Entramos en ataque" << std::endl;
        int id_victima = unidad.id_victima();
        //std::cout << "Id victima:  "<<id_victima <<std::endl;
        //std::cout << "Numero de cosechadoras:  "<<cosechadoras.count(id_victima) <<std::endl;
        if (tropas_muertas.count(id_victima) != 0/* ||
                (tropas.count(id_victima) == 0 &&
                cosechadoras.count(id_victima) == 0 &&
                edificios_atacados.count(id_victima) == 0)*/
            ){
            tropas.at(*it).parar_ataque();
            it = tropas_atacando.erase(it);
            continue;
        }
        //falta ver el tema del rango
        //std::cout << "Quiero atacar a :  "<<id_victima <<std::endl;
        int vida_nueva_victima = unidad.actualizar_ataque(dt,terreno);
        //std::cout << "atacamos" << std::endl;
        if (vida_nueva_victima <= 0) {
            matar_tropa(id_victima,*it);//lo saca del modelo
            it = tropas_atacando.erase(it);
            borrado = true;
        } else {
            comunicacion_jugadores.broadcast([&] (IJugador *j) {
                if (cosechadoras.count(id_victima) != 0)
                    j->atacar_tropa(id_victima,vida_nueva_victima);
                else if (tropas.count(id_victima) != 0)
                    j->atacar_tropa(id_victima,vida_nueva_victima);
                else
                    j->atacar_edificio(id_victima, vida_nueva_victima);
            });
        }
        if (!borrado)
            ++it;
    }
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
    eliminar_tropas();
}
unsigned int Ejercito::get_tiempo(std::string id_tipo){
    return prototipos.get_tiempo(id_tipo);
}
}