#ifndef _EJERCITO_H_
#define _EJERCITO_H_

#include <string>
#include <unordered_map>

#include "libs/json.hpp"

#include "cliente/modelo/infraestructura.h"
#include "cliente/modelo/terreno.h"
#include "cliente/modelo/tropa.h"

namespace cliente {

class Ejercito {
public:
    Ejercito(const nlohmann::json& ejercito, Infraestructura& inf, 
        Terreno& terreno, int id_jugador_actual_, const std::string& casa);

    /**
     * \brief Dibuja las tropas sobre la ventana.
     */
    void renderizar(Ventana& ventana, Camara& camara);
    
    /**
     * \brief Actualiza la posición de las tropas según corresponda.
     */
    void actualizar(int t_ms);

    /**
     * \brief Obtiene el número de sprite a mostrar para la clase indicada.
     */
    int obtener_sprite_clase(const std::string& clase) const;

    /**
     * \brief Devuelve true si la clase indicada está entrenando tropas
     */
    bool esta_entrenando(const std::string& clase) const;

    /**
     * \brief Devuelve true si la clase esta habilitada para su entrenamiento.
     */
    bool esta_habilitada(const std::string& clase) const;

    /**
     * \brief Obtiene la cantidad de tropas en la cola de la clase actual.
     */
    int obtener_cola_entrenamiento(const std::string& clase) const;

    /**
     * \brief Obtiene la cantidad de segundos restantes para que finalice el
     *        entrenamiento actual de la clase indicada.
     */
    int obtener_segundos_restantes(const std::string& clase) const;

    /**
     * \brief Obtiene las tropas base disponibles para entrenar.
     */
    std::vector<const Tropa*> obtener_tropas_base() const;

    bool existe(int id_tropa) const;

    Tropa& obtener(int id_tropa);

    const Tropa& obtener_tropa_base(const std::string& clase) const;

    void set_tropa_disparando(int id_tropa, bool disparando);

    /**
     * \brief Devuelve true si hay al menos una tropa enemiga en el punto dado.
     */
    bool hay_tropas_enemigas_en(const Posicion& punto);

    /**
     * \brief Obtiene una tropa enemiga parada en el punto indicado.
     * 
     * Si no hay tropas enemigas en el punto devuelve nullptr.
     */
    Tropa* obtener_tropa_enemiga_en(const Posicion& punto);

    /**
     * \brief Obtiene las tropas del jugador actual que estén en el área 
     *        indicada.
     */
    std::unordered_set<Tropa*> obtener_tropas_propias_en(
        const Rectangulo& area);

    
    /**** Eventos provenientes del servidor *****/

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
        const std::vector<int>& posicion, int id_jugador, int vida);
    
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
    std::string casa;
    Infraestructura& infraestructura;
    Terreno& terreno;
    std::unordered_map<int, Tropa> tropas;

    int last_ms = 0;

    float velocidad_ee = 1.0f;

    std::unordered_map<std::string, int> entrenamiento_actual;
    std::unordered_map<std::string, int> colas_entrenamiento;

    std::unordered_map<std::string, Tropa> tropas_base;
    std::vector<const Tropa*> tropas_base_ordenadas;

    int id_jugador_actual;
};

} // namespace cliente

#endif // _EJERCITO_H_
