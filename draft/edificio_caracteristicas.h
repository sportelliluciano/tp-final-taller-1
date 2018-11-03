#ifndef _EDIFICIOCARACTERISTICAS_H_
#define _EDIFICIOCARACTERISTICAS_H_

namespace modelo {

class EdificioCaracteristicas{
    private:
    char id;
    //Celda& posicion;
    int x;
    int y;
    unsigned int puntos_de_estructura;

    public:
    EdificioCaracteristicas(int x,int y,unsigned int puntos_de_estructura);
    ~EdificioCaracteristicas();
    bool reducir_ptos_est(dano);
    char get_id();
};
}
#endif