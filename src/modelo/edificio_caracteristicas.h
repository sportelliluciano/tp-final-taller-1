#ifndef _EDIFICIO_CARACTERISTICAS_H_
#define _EDIFICIO_CARACTERISTICAS_H_

namespace modelo {

class EdificioCaracteristicas{
    private:
    int id;
    //Celda& posicion;
    int x;
    int y;
    unsigned int puntos_de_estructura;

    public:
    EdificioCaracteristicas();
    EdificioCaracteristicas(int x,int y,unsigned int puntos_de_estructura);
    ~EdificioCaracteristicas();
    bool reducir_ptos_est(int dano);
    int get_id();
};
}
#endif
