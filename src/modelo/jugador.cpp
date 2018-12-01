#include "modelo/jugador.h"

#include <string>
#include <unordered_map>
#include <set>

#include "modelo/infraestructura.h"

#define ESCASES_ENERGIA 2
#define TIEMPO_CONSTRUCCION 5000

namespace modelo {

Jugador::Jugador(std::string casa_,IJugador* comunicacion_jugador_)
: casa(casa_), comunicacion_jugador(comunicacion_jugador_)
{ }
Jugador::~Jugador(){
}
void Jugador::aumentar_energia(unsigned int energia_){
    energia += energia_;
    comunicacion_jugador->actualizar_energia(energia,energia_max);
}
void Jugador::reducir_energia(unsigned int energia_){
    energia -= energia_;
    comunicacion_jugador->actualizar_energia(energia,energia_max);
}
void Jugador::aumentar_plata(unsigned int ganancia){
    plata += ganancia;
    comunicacion_jugador->actualizar_dinero(plata,energia_max);//cambiar energia max
}
void Jugador::reducir_plata(unsigned int costo){
    plata -= costo;
    comunicacion_jugador->actualizar_dinero(plata,energia_max);//cambiar energia max
}
std::string Jugador::get_casa(){
    return casa;
}
bool Jugador::hay_suficiente_energia(unsigned int costo){
    return energia >= costo;
}
bool Jugador::empezar_construccion(const std::string& clase,unsigned int costo){
    if (plata-costo < 0)
        return false;
    if (construcciones_en_cola.count(clase) == 0)
        construcciones_en_cola[clase] = 1;
    else
        construcciones_en_cola[clase]++;
        
    comunicacion_jugador->actualizar_cola_cc(clase, construcciones_en_cola[clase]);
    reducir_plata(costo);
    return true;
}
bool Jugador::cancelar_construccion(const std::string& clase, unsigned int costo){
    if (construcciones_en_cola.count(clase) != 0) {
        construcciones_en_cola[clase]--;
        comunicacion_jugador->actualizar_cola_cc(clase, construcciones_en_cola[clase]);
        if (construcciones_en_cola[clase] <= 0)
            construcciones_en_cola.erase(clase);
        aumentar_plata(costo);
        return true;
    }
    return false;
}

bool Jugador::empezar_entrenamiento(const std::string& clase,unsigned int costo)
{   
    if (plata-costo < 0)
        return false;
    if (tropas_en_cola.count(clase) == 0)
            tropas_en_cola[clase] = 1;
    else
        tropas_en_cola[clase]++;
        
    comunicacion_jugador->actualizar_cola_ee(clase, tropas_en_cola[clase]);
    reducir_plata(costo);
    return true;
}
bool Jugador::cancelar_entrenamiento(const std::string& clase, unsigned int costo){
    if (tropas_en_cola.count(clase) != 0) {
        tropas_en_cola[clase]--;
        comunicacion_jugador->actualizar_cola_ee(clase, tropas_en_cola[clase]);
        if (tropas_en_cola[clase] <= 0)
            tropas_en_cola.erase(clase);
        aumentar_plata(costo);
        return true;
    }
    return false;
}

bool Jugador::tiene(std::set<std::string>& requisitos,Infraestructura& inf){
    unsigned int cumplidos = 0;
    for (auto it = requisitos.begin();it!=requisitos.end();++it){
        for (auto it2 = inventario.begin();it2!=inventario.end();++it2){
            if (inf.pertenece(*it2)){
                if (inf.get(*it2).get_tipo() == *it ){
                    cumplidos++;
                    break;
                }
            }
        }
    }
    if (requisitos.size() == cumplidos)
        return true;
    return false;     
}
float Jugador::obtener_varaible_de_entrenamiento(std::set<std::string>& requisitos,Infraestructura& inf){
    unsigned int cumplidos = 0;
    for (auto it = requisitos.begin();it!=requisitos.end();++it){
        for (auto it2 = inventario.begin();it2!=inventario.end();++it2){
            if (inf.pertenece(*it2)){
                if (inf.get(*it2).get_tipo() == *it ){
                    cumplidos++;
                }
            }
        }
    }
    int excedente = cumplidos - requisitos.size();
    if (!excedente)
        return 1;
    float variable = 1/(excedente+1) *1.8;
    if (variable < 0.25)//cota
        return 0.25;
    return variable;
}

void Jugador::agregar_elemento(int id,unsigned int energia_,const std::string& clase){
    inventario.insert(id);
    //si es un edificio
    if (construcciones_esperando_ubicacion.count(clase)!=0)
        construcciones_esperando_ubicacion.erase(clase);
    reducir_energia(energia_);
}

void Jugador::eliminar_elemento(int id,unsigned int energia_consumida){
    inventario.erase(id);
    aumentar_energia(energia_consumida);
}

bool Jugador::pertenece(int id){
    return inventario.count(id)>0;
}

bool Jugador::ubicar_edificio(const std::string& clase, int celda_x, 
    int celda_y, Infraestructura& inf) 
{
    if (construcciones_esperando_ubicacion.count(clase) == 0)
        return false;
    
    //if (!hay_suficiente_energia(inf.get_energia(clase)))
    //    return false;

    construcciones_esperando_ubicacion.erase(clase);
    
    int id_edificio = inf.crear(clase, celda_x, celda_y, 
        comunicacion_jugador->obtener_id());
    unsigned int energia_ = inf.get_energia(clase);
    if (clase == "trampas_aire"){
        agregar_elemento(id_edificio, 0, clase);
        aumentar_energia(energia_);
    } else{
        agregar_elemento(id_edificio, energia_, clase);
    }
    return true;
}

void Jugador::actualizar_construcciones(int dt,Infraestructura& inf) {
    for (auto it=construcciones.begin(); it != construcciones.end();) {
        if (it->second - dt < 0) {
            construcciones_esperando_ubicacion.insert(it->first);
                
            // Construccion terminada
            comunicacion_jugador->sincronizar_construccion(it->first, 0);
            
            it = construcciones.erase(it);
        } else {
            it->second -= dt;
            ++it;
        }
    }

    for (auto it = construcciones_en_cola.begin(); it != construcciones_en_cola.end();) 
    {   //se procesa cada construccion a la vez
        if ((construcciones.count(it->first) == 0) && 
           (construcciones_esperando_ubicacion.count(it->first) == 0)) 
        {
            int tiempo;
            if (energia <= 0)
                tiempo = TIEMPO_CONSTRUCCION*ESCASES_ENERGIA;
            tiempo = TIEMPO_CONSTRUCCION;
            construcciones[it->first] = tiempo;//va el tiempo de construccion
            comunicacion_jugador->iniciar_construccion(it->first, tiempo);
            it->second--;
            comunicacion_jugador->actualizar_cola_cc(it->first, it->second);
            if (it->second == 0) {
                it = construcciones_en_cola.erase(it);
                continue;
            }
        } 
        ++it;
    }
}

void Jugador::actualizar_entrenamientos(int dt,std::vector<std::string>& nuevas_tropas,
                                    std::unordered_map<std::string,int>& tiempos_de_entrenamiento) {
    nuevas_tropas.clear();
    for (auto it=tropas.begin(); it != tropas.end();) {
        if (it->second - dt < 0) {
            // Tropa entrenada
            comunicacion_jugador->sincronizar_entrenamiento(it->first, 0);
            const std::string& id_tipo = it->first;
            nuevas_tropas.push_back(id_tipo);
            it = tropas.erase(it);
        } else {
            it->second -= dt;
            ++it;
        }
    }

    for (auto it = tropas_en_cola.begin(); it != tropas_en_cola.end();) 
    {   //se procesa cada construccion a la vez
        if ((tropas.count(it->first) == 0))
        {
            int tiempo;
            if (energia <= 0)
                tiempo = tiempos_de_entrenamiento.at(it->first)*ESCASES_ENERGIA;
            tiempo = tiempos_de_entrenamiento.at(it->first);
            tropas[it->first] = tiempo/8;
            comunicacion_jugador->iniciar_entrenamiento(it->first, tiempo/8);
            it->second--;
            comunicacion_jugador->actualizar_cola_ee(it->first, it->second);
        }
        if (it->second == 0) {
            it = tropas_en_cola.erase(it);
        } else {
            ++it;
        }
    }
}
IJugador* Jugador::get_jugador(){
    return comunicacion_jugador;
}

bool Jugador::inicio_sincronizado() const {
    return sincronizado;
}

void Jugador::inicio_sincronizado(bool activar) {
    sincronizado = activar;
}

}