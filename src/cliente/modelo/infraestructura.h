#ifndef _INFRAESTRUCTURA_H_
#define _INFRAESTRUCTURA_H_

#include <list>
#include <unordered_map>

#include "cliente/modelo/celda.h"
#include "cliente/modelo/edificio.h"
#include "cliente/modelo/terreno.h"

namespace cliente {

class Infraestructura {
public:
    Infraestructura(Terreno& terreno);
    void renderizar(Ventana& ventana);
    void actualizar(int t_ms);
    void construir(int id, const std::string& clase, int x, int y);
    void destruir(int id);
    std::vector<const Edificio*> obtener_edificios() const;
    void iniciar_construccion(const std::string& clase);
    void seleccionar(const Edificio& edificio);
    void limpiar_seleccion();

private:
    Terreno& terreno;
    std::unordered_map<int, Edificio> edificios_construidos;

    std::unordered_map<std::string, Edificio> edificios;

    std::unordered_map<std::string, std::list<int>> cola_construccion;

    Edificio* edificio_seleccionado = nullptr;
};

} // namespace cliente

#endif // _INFRAESTRUCTURA_H_
