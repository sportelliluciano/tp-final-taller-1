#ifndef _EJERCITO_H_
#define _EJERCITO_H_

#include "cliente/modelo/terreno.h"

namespace cliente {

class Ejercito {
public:
    Ejercito(Terreno& terreno);
    void renderizar(Ventana& ventana);
    
private:
    Terreno& terreno;
};

} // namespace cliente

#endif // _EJERCITO_H_
