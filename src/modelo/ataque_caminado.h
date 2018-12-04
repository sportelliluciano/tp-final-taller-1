#ifndef _ATAQUE_CAMINADO_H_
#define _ATAQUE_CAMINADO_H_

#include "modelo/arma_caracteristicas.h"

namespace modelo {

//comportamiento extra
class AtaqueCaminado:public ArmaCaracteristicas{
    private:
    public:
    AtaqueCaminado();
    ~AtaqueCaminado();
    void aplicar();
};

}

#endif //_ATAQUE_CAMINADO_H_
