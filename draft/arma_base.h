#ifndef _ARMABASE_H_
#define _ARMABASE_H_

namespace modelo {

class ArmaBase{
    private:
    char tipo;
    unsigned int dano;
    unsigned int frecuencia;
    
    public:
    ArmaBase();
    ~ArmaBase()
};
}
#endif