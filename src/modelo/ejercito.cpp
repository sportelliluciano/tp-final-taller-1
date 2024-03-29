#include "modelo/ejercito.h"

#include <unordered_map>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <utility>
#include <vector>
#include <set>

#include "modelo/terreno.h"
#include "modelo/unidad.h"
#include "modelo/ejercito_creador.h"
#include "modelo/arma_creador.h"
#include "modelo/edificio.h"
#include "modelo/id.h"
#include "modelo/broadcaster.h"
#include "modelo/gusano.h" 

namespace modelo {
    
Ejercito::Ejercito(Broadcaster& broadcaster,Id& id) 
: comunicacion_jugadores(broadcaster),id_(id) { 
}

void Ejercito::inicializar(Terreno* terreno_,const nlohmann::json& ejercito_){
    prototipos.inicializar(terreno_,ejercito_);
    terreno=terreno_;
    prototipos.get_tiempos_de_entrenamiento(tiempos_de_entrenamiento);
}

Ejercito::~Ejercito(){
}

std::unordered_map<std::string,int>& Ejercito::get_tiempos_entrenamiento(){
    return tiempos_de_entrenamiento;
}

int Ejercito::crear(const std::string& id_tipo, int id_propietario) {
    Posicion posicion = terreno->obtener_centro_posicion(id_propietario);
    posicion = terreno->obtener_posicion_caminable_cercana(posicion);
    if (!(terreno->rango_valido_tropa(posicion.x(),posicion.y(),
        prototipos.get_dimensiones(id_tipo)))) return 0; //raise error
    int nuevo_id = id_.nuevo_id();
    tropas.emplace(nuevo_id,prototipos.clonar(id_tipo,
            nuevo_id,posicion.x(),posicion.y()));
    terreno->agregar_tropa(posicion, prototipos.get_dimensiones(id_tipo));
    comunicacion_jugadores.broadcast([&] (IJugador* j) {
        j->crear_tropa(nuevo_id, id_tipo, posicion.px_x(), posicion.px_y(),
            prototipos.get_vida(id_tipo), id_propietario);
    });
    return nuevo_id;
}

int Ejercito::crear_cosechadora(const std::string& id_tipo,int id_propietario,
                                                Jugador* comunicacion_jugador){
    Posicion posicion = terreno->obtener_centro_posicion(id_propietario);
    posicion = terreno->obtener_posicion_caminable_cercana(posicion);
    if (!(terreno->rango_valido_tropa(posicion.x(),posicion.y(),
        prototipos.get_dimensiones(id_tipo)))) return 0; //raise error
    int nuevo_id = id_.nuevo_id();
    terreno->agregar_tropa(posicion, prototipos.get_dimensiones(id_tipo));
    cosechadoras.emplace(nuevo_id,prototipos.clonar(
            id_tipo,nuevo_id,posicion.x(),posicion.y(),
            terreno,id_propietario,comunicacion_jugador));
    
    Posicion especia = terreno->obtener_especia_cercana(posicion);
    especia = terreno->obtener_posicion_caminable_cercana(especia);
    especia = Posicion(especia.x(),especia.y());
    
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
    
    for (auto it = a_estrella.begin(); it!= a_estrella.end(); ++it) {
        v.emplace_back(std::pair<int,int>((*it).px_x(), (*it).px_y()));
    }
    
    comunicacion_jugadores.broadcast([&] (IJugador *j) {
        j->mover_tropa(id,v);
    });
}

void Ejercito::mover_cosechadora(int id,int x,int y){
    Cosechadora& cosechadora = cosechadoras.at(id);
    std::vector<Posicion> a_estrella =
            terreno->buscar_camino_minimo(cosechadora.get_posicion(),
                                                        Posicion(x,y));
    cosechadora.configurar_camino(a_estrella);
    tropas_en_movimiento.insert(id);
    std::vector<std::pair<int,int>> v;
    for (auto it = a_estrella.begin(); it!= a_estrella.end(); ++it){
        v.emplace_back(std::pair<int,int>((*it).px_x(),(*it).px_y()));
    }
    comunicacion_jugadores.broadcast([&] (IJugador *j) {
        j->mover_tropa(id,v);
    });
}
 void Ejercito::atacar(int id_victima,int id_atacante){
     bool caminar = false;
     Atacable* victima; 
     if (tropas.count(id_atacante)!=0 && tropas.count(id_victima)!=0){
         victima = &(tropas.at(id_victima));
         if (tropas.at(id_atacante).configurar_ataque(victima)){
             tropas_atacando.insert(id_atacante);
         } else{
             caminar = true;
         }
     }
     if (tropas.count(id_atacante)!=0 && cosechadoras.count(id_victima)!=0){
         victima = &(cosechadoras.at(id_victima));
         if (tropas.at(id_atacante).configurar_ataque(victima)){
             tropas_atacando.insert(id_atacante);
         } else{
             caminar = true;
         }
     }
     if (caminar){
         Posicion& posicion = victima->get_posicion();
         mover(id_atacante,posicion.x(),posicion.y());
     }
 }

void Ejercito::atacar(Atacable* edificio,int id_atacante){
    // acercarme hasta cumplir con el rango 
    if (tropas.count(id_atacante)!=0) {
        if (tropas.at(id_atacante).configurar_ataque(edificio)){
            tropas_atacando.insert(id_atacante);
        } else {
            Posicion& posicion = edificio->get_posicion();
            mover(id_atacante,posicion.x(),posicion.y());
        }
    }
}
Unidad& Ejercito::get(int id) {
    if (cosechadoras.count(id)!=0){
        return cosechadoras.at(id);
    } else{
        return tropas.at(id);
    }
}
std::set<std::string>& Ejercito::get_requisitos(const std::string& clase){
    return prototipos.get_requisitos(clase);
}

bool Ejercito::pertenece(const std::string& clase,const std::string& casa){
    std::set<std::string>& casas = prototipos.get_casas(clase);
    return casas.count(casa)!=0;
}

unsigned int Ejercito::get_costo(const std::string& id_tipo) {
    return prototipos.get_costo(id_tipo);
}

void Ejercito::matar_tropa(int id_victima,int id_atacante){
    tropas_muertas.insert(id_victima);
    if (tropas.count(id_atacante) != 0)//puede ser el gusano
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
            it != tropas_muertas.end(); ++it)
    {
        if (tropas.count(*it) != 0){
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

void Ejercito::actualizar_cosechadoras(int dt,Cosechadora& cosechadora){
    if (cosechadora.operando()){
            cosechadora.operar(dt);
    } else if (!cosechadora.en_movimiento()){
        try{ //por si no hay posicion (no hay refinerias o no hay mas especia)
        Posicion posicion;
        if (cosechadora.camino_a_especia()){
            posicion=
                terreno->obtener_especia_cercana(cosechadora.get_posicion());
            posicion = Posicion(posicion.x(),posicion.y());
        } else {
            posicion = terreno->obtener_refinerias_cercana(
                                            cosechadora.get_posicion(),
                                            cosechadora.obtener_id_jugador());
        }
        posicion = terreno->obtener_posicion_caminable_cercana(posicion); 
        mover_cosechadora(cosechadora.get_id(),posicion.x(),posicion.y());
        std::cout << "en camino"<< std::endl;
        } catch(std::runtime_error& e)
        {
            return;
        }
    } else{
        if (cosechadora.actualizar_posicion(dt,terreno)) {
            comunicacion_jugadores.broadcast([this, &cosechadora](IJugador *j){
            j->sincronizar_tropa(cosechadora.get_id(), 
                cosechadora.get_posicion().px_x(),
                cosechadora.get_posicion().px_y());
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
                    unidad.get_posicion().px_y());
            });
        }
        ++it;
    }
}

Atacable& Ejercito::obtener_unidad_aleatorio(){
    float bernulli = rand()/ RAND_MAX;
    if (bernulli <= 0.5 && tropas.size() != 0){
        int indice_vicitma = rand() % tropas.size();
        auto victima = tropas.begin();
        std::advance(victima,indice_vicitma);
        return (victima->second);
    } else if (cosechadoras.size() != 0){
        int indice_vicitma = rand() % cosechadoras.size();
        auto victima = cosechadoras.begin();
        std::advance(victima,indice_vicitma);
        return (victima->second);
    }
    throw std::runtime_error("No hay unidades");
}

void Ejercito::actualizar_gusano(int dt){
    if (gusano.esperando_comer()){
        try {
        Atacable& victima = obtener_unidad_aleatorio();
        matar_tropa(victima.get_id(),gusano.get_id());
        Posicion& pos = victima.get_posicion();
        comunicacion_jugadores.broadcast([&] (IJugador *j) {
                j->mostrar_gusano(pos.x(),pos.y());
            });
        } catch(std::runtime_error& e){
            std::cout << "no hay comida "<<std::endl;
            gusano.comer();
            return;    
        }

        gusano.comer();
    } else {
        gusano.actualizar(dt);
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
            //aquellos que atacaron y no mataron a su 
            //victima pero despues alguien mas la mato
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
            it != tropas_atacando.end(); )
    {
        bool borrado = false;
        if (tropas_muertas.count(*it)!= 0) {
            it = tropas_atacando.erase(it);
            continue;
        }
        // el verificar la muerte aca hace que
        // limpiar_tropas_atacado sea mas rapido
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
        try{
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
        } catch(std::runtime_error& e){
            it = tropas_atacando.erase(it);
            borrado = true;
        }
        if (!borrado)
            ++it;
    }
    actualizar_gusano(dt);
    //saco a los muertos restantes del set
    limpiar_tropas_atacando();
    eliminar_tropas();
}

void Ejercito::actualizar_tropas(int dt) {
    actualizar_movimiento(dt);
    actualizar_ataques(dt);
}
unsigned int Ejercito::get_tiempo(const std::string& id_tipo){
    return prototipos.get_tiempo(id_tipo);
}
} // namespace modelos
