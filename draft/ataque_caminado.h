#ifndef _ATAQUECAMINADO_H_
#define _ATAQUECAMINADO_H_

#include "arma_caracteristicas.h"

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
#endif
