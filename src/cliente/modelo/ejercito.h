#ifndef _EJERCITO_H_
#define _EJERCITO_H_

#include <unordered_map>

#include "cliente/modelo/terreno.h"
#include "cliente/modelo/tropa.h"

namespace cliente {

class Ejercito {
public:
    Ejercito(Terreno& terreno);

    /**
     * \brief Dibuja las tropas sobre la ventana.
     */
    void renderizar(Ventana& ventana);
    
    /**
     * \brief Actualiza la posición de las tropas según corresponda.
     */
    void actualizar(int t_ms);

    /**
     * \brief Inicia el entrenamiento de una clase de tropa.
     */
    void entrenar(const std::string& clase, int tiempo_ms);
    
    /**
     * \brief Sincroniza los tiempos de entrenamiento de una clase de tropa.
     */
    void sincronizar_entrenamiento(const std::string& clase, int tiempo_ms);
    
    /**
     * \brief Actualiza la cola de entrenamiento de una clase de tropa.
     */
    void actualizar_cola_entrenamiento(const std::string& clase, int cantidad);
    
    /**
     * \brief Crea una nueva tropa sobre el mapa.
     */
    void crear_tropa(int id, const std::string& clase, 
        const std::vector<int>& posicion, int id_jugador);
    
    /**
     * \brief Inicia el movimiento de una tropa siguiendo el camino indicado.
     * camino es un vector de enteros donde las posiciones pares indican
     * la coordenada x del paso actual y las posiciones impares la coordenada
     * y del mismo.
     * El primer paso debería ser la posición actual.
     */
    void mover_tropa(int id, const std::vector<int>& camino);
    
    /**
     * \brief Sincroniza el movimiento de una tropa con la posición en el 
     * servidor y permite que continúe su movimiento.
     */
    void sincronizar_tropa(int id, const std::vector<int>& posicion);
    
    /**
     * \brief Ataca a la tropa indicada.
     */
    void atacar_tropa(int id, int nueva_vida);
    
    /**
     * \brief Elimina la tropa indicada del tablero.
     */
    void destruir_tropa(int id);

private:
    Terreno& terreno;
    std::unordered_map<int, Tropa> tropas;
    std::unordered_set<int> unidades_seleccionadas;

    int last_ms = 0;

    std::unordered_map<std::string, int> entrenamiento_actual;
    std::unordered_map<std::string, int> colas_entrenamiento;
};

} // namespace cliente

#endif // _EJERCITO_H_
