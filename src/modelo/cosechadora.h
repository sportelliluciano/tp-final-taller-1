#ifndef _COSECHADORA_H_
#define _COSECHADORA_H_

#include <vector>
#include <unordered_map>

#include "modelo/terreno.h"
#include "modelo/unidad.h"
#include "modelo/unidad_base.h"
#include "conexion/i_jugador.h"

namespace modelo {

class Cosechadora:public Unidad {
    private:
    Terreno& terreno_;
    bool descargando=false;
    int tiempo_descarga = 5000;
    bool camino_especia = true;
    IJugador* jugador;

    public:
    Cosechadora(int id,int pos_x,int pos_y, UnidadBase& unidad_base,
        Terreno& terreno,IJugador* jugador);
    ~Cosechadora();
    void actualizar_posicion(int dt,Terreno terreno,
                std::vector<IJugador*>& jugadores);
    bool camino_a_especia();
    IJugador* obtener_jugador();
};
}
#endif