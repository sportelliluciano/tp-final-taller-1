#ifndef _INFRAESTRUCTURA_H_
#define _INFRAESTRUCTURA_H_

#include <unordered_map>

#include "cliente/modelo/celda.h"
#include "cliente/modelo/edificio.h"
#include "cliente/modelo/terreno.h"

namespace cliente {

class Infraestructura {
public:
    Infraestructura(Terreno& terreno);
    void renderizar(Ventana& ventana);
    void construir(int id, const std::string& clase, int x, int y);
    void destruir(int id);

private:
    Terreno& terreno;
    std::unordered_map<int, Edificio> edificios_construidos;

    std::unordered_map<std::string, Edificio> edificios;
};

} // namespace cliente

#endif // _INFRAESTRUCTURA_H_
