#ifndef _COSECHADORA_H_
#define _COSECHADORA_H_

#include "modelo/terreno.h"
#include "modelo/unidad.h"
#include "modelo/unidad_base.h"
#include "modelo/jugador.h"
#include "comun/i_jugador.h"

#define ESPERA 10000 

namespace modelo {

class Cosechadora:public Unidad {
    private:
    Terreno* terreno_;
    bool operando_ = false; // cargando o descargando especia
    int tiempo_descarga = ESPERA;
    bool camino_especia = true;
    bool llegue = false;
    int id_propietario;
    Jugador* comunicacion_jugador;

    public:
    Cosechadora(int id,int pos_x,int pos_y, UnidadBase& unidad_base,
        Terreno* terreno,int id_propietario,Jugador* comunicacion_jugador);
    ~Cosechadora();
    bool actualizar_posicion(int dt,Terreno* terreno);
    bool camino_a_especia();
    bool operando();
    void operar(int ds);
    
    int obtener_id_jugador();
};
}
#endif
