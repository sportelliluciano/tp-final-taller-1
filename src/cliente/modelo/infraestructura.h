#ifndef _INFRAESTRUCTURA_H_
#define _INFRAESTRUCTURA_H_

#include "cliente/modelo/terreno.h"
#include "cliente/modelo/edificio.h"

namespace cliente {

class Infraestructura {
public:
    Infraestructura(Terreno& terreno);
    void renderizar(Ventana& ventana);
private:
    Terreno& terreno;
    std::vector<Edificio*> edificios;
};

} // namespace cliente

#endif // _INFRAESTRUCTURA_H_
