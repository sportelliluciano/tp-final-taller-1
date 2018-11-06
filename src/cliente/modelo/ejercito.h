#ifndef _EJERCITO_H_
#define _EJERCITO_H_

#include "cliente/modelo/terreno.h"
#include "cliente/modelo/tropa.h"

namespace cliente {

class Ejercito {
public:
    Ejercito(Terreno& terreno);
    void renderizar(Ventana& ventana);
    
private:
    Terreno& terreno;
    Tropa tropa;
};

} // namespace cliente

#endif // _EJERCITO_H_
