#include "modelo/jugador.h"

#include <string>
#include <unordered_map>
#include <set>

#include "infraestructura.h"

namespace modelo {

Jugador::Jugador(std::string casa_,IJugador* comunicacion_jugador_):
                                casa(casa_),
                                comunicacion_jugador(comunicacion_jugador_){
}
Jugador::~Jugador(){
}
void Jugador::aumentar_consumo(unsigned int consumo_){
    consumo += consumo_;
}
void Jugador::reducir_consumo(unsigned int consumo_){
    consumo -= consumo_;
}
void Jugador::aumentar_energia(unsigned int energia_){
    energia += energia_;
    comunicacion_jugador->actualizar_energia(energia,energia_max);
}
void Jugador::reducir_energia(unsigned int energia_){
    energia -= energia_;
    comunicacion_jugador->actualizar_energia(energia,energia_max);
}
std::string Jugador::get_casa(){
    return casa;
}
bool Jugador::hay_suficiente_energia(unsigned int costo){
    return energia >= costo;
}
bool Jugador::empezar_construccion(const std::string& clase,unsigned int costo){
    if (construcciones_en_cola.count(clase) == 0)
            construcciones_en_cola[clase] = 1;
    else
        construcciones_en_cola[clase]++;
        
    comunicacion_jugador->actualizar_cola_cc(clase, construcciones_en_cola[clase]);
    reducir_energia(costo);
    return true;
}
bool Jugador::cancelar_construccion(const std::string& clase, unsigned int costo){
    if (construcciones_en_cola.count(clase) != 0) {
        construcciones_en_cola[clase]--;
        comunicacion_jugador->actualizar_cola_cc(clase, construcciones_en_cola[clase]);
        if (construcciones_en_cola[clase] <= 0)
            construcciones_en_cola.erase(clase);
        aumentar_energia(costo);
        return true;
    }
    return false;
}

bool Jugador::empezar_entrenamiento(const std::string& clase,unsigned int costo){
    if (tropas_en_cola.count(clase) == 0)
            tropas_en_cola[clase] = 1;
    else
        tropas_en_cola[clase]++;
        
    comunicacion_jugador->actualizar_cola_cc(clase, tropas_en_cola[clase]);
    reducir_energia(costo);
    return true;
}
bool Jugador::cancelar_entrenamiento(const std::string& clase, unsigned int costo){
    if (tropas_en_cola.count(clase) != 0) {
        tropas_en_cola[clase]--;
        comunicacion_jugador->actualizar_cola_cc(clase, tropas_en_cola[clase]);
        if (tropas_en_cola[clase] <= 0)
            tropas_en_cola.erase(clase);
        aumentar_energia(costo);
        return true;
    }
    return false;
}

void Jugador::agregar_elemento(int id,unsigned int energia_,const std::string& clase){
    inventario.insert(id);
    //si es un edificio
    if (construcciones_esperando_ubicacion.count(clase)!=0)
        construcciones_esperando_ubicacion.erase(clase);
    consumo += energia_;
}

void Jugador::eliminar_elemento(int id,unsigned int energia_consumida){
    consumo -= energia_consumida;
    inventario.erase(id);
}

bool Jugador::pertenece(int id){
    return inventario.count(id)>0;
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
            construcciones[it->first] = 5000;//va el tiempo de construccion
            comunicacion_jugador->iniciar_construccion(it->first, 5000);
            it->second--;
            comunicacion_jugador->actualizar_cola_cc(it->first, it->second);
        }
        if (it->second == 0) {
            it = construcciones_en_cola.erase(it);
        } else {
            ++it;
        }
    }
}

std::string Jugador::actualizar_tropas(int dt,Ejercito& ejercito) {
    for (auto it=tropas.begin(); it != tropas.end();) {
        if (it->second - dt < 0) {
            // Construccion terminada
            comunicacion_jugador->sincronizar_construccion(it->first, 0);
            
            it = tropas.erase(it);
            return(it->first);
        } else {
            it->second -= dt;
            ++it;
        }
    }

    for (auto it = tropas_en_cola.begin(); it != tropas_en_cola.end();) 
    {   //se procesa cada construccion a la vez
        if ((tropas.count(it->first) == 0)) 
        {
            tropas[it->first] = ejercito.get_tiempo(it->first);
            comunicacion_jugador->iniciar_construccion(it->first, ejercito.get_tiempo(it->first));//tropa
            it->second--;
            comunicacion_jugador->actualizar_cola_cc(it->first, it->second);
        }
        if (it->second == 0) {
            it = tropas_en_cola.erase(it);
        } else {
            ++it;
        }
    }
    return std::string("ok");
}

}