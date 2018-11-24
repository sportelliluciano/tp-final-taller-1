#ifndef _COSECHADORA_H_
#define _COSECHADORA_H_

#include <vector>
#include <unordered_map>

#include "modelo/terreno.h"
#include "modelo/unidad.h"
#include "modelo/unidad_base.h"
#include "conexion/i_jugador.h"

#define ESPERA 10000 

namespace modelo {

class Cosechadora:public Unidad {
    private:
    Terreno* terreno_;
    bool operando_ = false; // cargando o descargando especia
    int tiempo_descarga = ESPERA;
    bool camino_especia = true;
    bool llegue = false;
    //IJugador* jugador;

    public:
    Cosechadora(int id,int pos_x,int pos_y, UnidadBase& unidad_base,
        Terreno* terreno);
    ~Cosechadora();
    bool actualizar_posicion(int dt,Terreno* terreno);
    bool camino_a_especia();
    bool operando();
    void operar(int ds);
    //IJugador* obtener_jugador();
};
}
#endif