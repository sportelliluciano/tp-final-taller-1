#ifndef _ARMA_CARACTERISTICAS_H_
#define _ARMA_CARACTERISTICAS_H_

namespace modelo {

/**
 * \brief ArmaCaracteristicas
 * 
 * Encampsula el comportamiento extra o especifico de un arma. 
 */
class ArmaCaracteristicas{
    private:
    public:
    virtual ~ArmaCaracteristicas(){}
    virtual void aplicar();
};

}// namespace modelo

#endif // _ARMA_CARACTERISTICAS_H_
