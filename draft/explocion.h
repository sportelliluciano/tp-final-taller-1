#ifndef _EXPLOCION_H_
#define _EXPLOCION_H_

#include "arma_caracteristicas.h"

namespace modelo {

//comportamiento extra
class Explocion:public ArmaCaracteristicas{
    private:

    public:
    Explocion();
    ~Explocion();
    void aplicar();
};
}
#endif
