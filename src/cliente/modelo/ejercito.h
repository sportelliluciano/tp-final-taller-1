#ifndef _EJERCITO_H_
#define _EJERCITO_H_

#include <unordered_map>

#include "cliente/modelo/terreno.h"
#include "cliente/modelo/tropa.h"

namespace cliente {

class Ejercito {
public:
    Ejercito(Terreno& terreno);
    void renderizar(Ventana& ventana);
    /**
     * \brief Actualiza la posición de las tropas según corresponda.
     */
    void actualizar(int t_ms);
    void crear(int id, const std::string& clase, int x, int y);
    void mover(int id, int x_destino, int y_destino);
    void seleccionar(const std::unordered_set<const Tropa*>& unidades);
    void destruir(int id);
    void sincronizar_tropa(int id_tropa, int x, int y);
    void indicar_camino_tropa(int id_tropa, 
        const std::vector<std::pair<int, int>>& camino);
    
private:
    Terreno& terreno;
    std::unordered_map<int, Tropa> tropas;
    std::unordered_set<int> unidades_seleccionadas;

    int last_ms = 0;
};

} // namespace cliente

#endif // _EJERCITO_H_
