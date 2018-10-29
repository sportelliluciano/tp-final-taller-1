#ifndef _INFRAESTRUCTURA_H_
#define _INFRAESTRUCTURA_H_

#include "cliente/modelo/celda.h"
#include "cliente/modelo/edificio.h"
#include "cliente/modelo/terreno.h"


namespace cliente {

class Infraestructura {
public:
    Infraestructura(Terreno& terreno);
    void renderizar(Ventana& ventana);
    void construir(const std::string& clase, int x, int y);

private:
    Terreno& terreno;
    std::vector<Edificio> edificios_construidos;

    std::unordered_map<std::string, Edificio> edificios;
};

} // namespace cliente

#endif // _INFRAESTRUCTURA_H_
