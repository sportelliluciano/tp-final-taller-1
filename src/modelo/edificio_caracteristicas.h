#ifndef _EDIFICIO_CARACTERISTICAS_H_
#define _EDIFICIO_CARACTERISTICAS_H_

#include "modelo/posicion.h"

namespace modelo {

class EdificioCaracteristicas{
    private:
    int id;
    //Celda& posicion;
    Posicion posicion;
    unsigned int puntos_de_estructura;

    public:
    EdificioCaracteristicas();
    EdificioCaracteristicas(int id,int x,int y,unsigned int puntos_de_estructura);
    ~EdificioCaracteristicas();
    bool reducir_ptos_est(int dano);
    int get_id();
    int get_vida();
    Posicion& get_posicion()const;
};
}
#endif
