#ifndef _ARMA_CARACTERISTICAS_H_
#define _ARMA_CARACTERISTICAS_H_

namespace modelo {

//comportamiento extra
class ArmaCaracteristicas{
    private:

    public:
    virtual ~ArmaCaracteristicas(){}
    virtual void aplicar();
};
}
#endif