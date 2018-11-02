#ifndef _EDIFICIOBASE_H_
#define _EDIFICIOBASE_H_

namespace modelo {

class EdificioBase{
    private:
    char tipo;
    unsigned int energia;
    unsigned int costo;
    unsigned int dimensiones;
    unsigned int puntos_de_estructura;
    
    public:
    EdificioBase();
    ~EdificioBase()
};
}
#endif