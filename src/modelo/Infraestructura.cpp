#include "modelo/Infraestructura.h"

void Infraestructura::Crear(int x, int y, char id){ 
}
void Infraestructura::crear_centro_de_construccion(int x, int y);
void Infraestructura::crear_fabrica_ligera(int x, int y);
void Infraestructura::crear_fabrica_pesada(int x, int y);
void Infraestructura::crear_palacio(int x, int y);
void Infraestructura::crear_silo(int x, int y);
void Infraestructura::crear_cuartel(int x, int y);
void Infraestructura::crear_trampa_de_aire(int x, int y);
void Infraestructura::destruir_centro_de_construccion(char id);
void Infraestructura::destruir_fabrica_ligera(char id);
void Infraestructura::destruir_fabrica_pesada(char id);
void Infraestructura::destruir_palacio(char id);
void Infraestructura::destruir_silo(char id);
void Infraestructura::destruir_cuartel(char id);
void Infraestructura::destruir_trampa_de_aire(char id);
bool Infraestructura::centro_de_construccion(){
    return centro_de_construccion > 0;
}
bool Infraestructura::fabrica_ligera(){
    return fabrica_ligera > 0;
}
bool Infraestructura::fabrica_pesada(){
    return fabrica_pesada > 0;
}
bool Infraestructura::palacio(){
    return palacio > 0;
}
bool Infraestructura::silo(){
    return silo > 0;
}
bool Infraestructura::cuartel(){
    return cuartel > 0;
}
bool Infraestructura::trampa_de_aire(){
    return trampa_de_aire > 0;
}