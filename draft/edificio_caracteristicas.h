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
    EdificioCaracteristicas();
    ~EdificioCaracteristicas()
};
}
#endif