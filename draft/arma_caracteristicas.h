#ifndef _ARMACARACTERISTICAS_H_
#define _ARMACARACTERISTICAS_H_

namespace modelo {

//comportamiento extra
class ArmaCaracteristicas{
    private:

    public:
    virtual ~ArmaCaracteristicas(){}
    virtual void aplicar() = 0;
};
}
#endif