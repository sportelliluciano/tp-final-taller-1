#ifndef _HIPNOSIS_H_
#define _HIPNOSIS_H_

#include "modelo/arma_caracteristicas.h"

namespace modelo {

//comportamiento extra
class Hipnosis:public ArmaCaracteristicas{
    private:

    public:
    Hipnosis();
    ~Hipnosis();
    void aplicar();
};
}
#endif
