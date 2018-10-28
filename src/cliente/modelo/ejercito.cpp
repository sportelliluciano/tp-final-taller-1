#include "cliente/modelo/ejercito.h"

#include "cliente/modelo/terreno.h"

namespace cliente {

Ejercito::Ejercito(Terreno& terreno_juego) : terreno(terreno_juego) { }

void Ejercito::renderizar(Ventana& ventana) {
    std::vector<Celda> celdas_visibles = 
        terreno.obtener_celdas_visibles(ventana);
    
    for (auto it = celdas_visibles.begin(); it != celdas_visibles.end(); ++it) {
        const Celda& celda = *it;

        if (!celda.contiene_tropas())
            continue;
        
        std::vector<Tropa*> tropas = celda.obtener_tropas();

    }
}

} // namespace cliente
