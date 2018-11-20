#ifndef _JUEGO_H_
#define _JUEGO_H_

#include <string>
#include <vector>

#include "libs/json.hpp"

#include "cliente/modelo/ejercito.h"
#include "cliente/modelo/gusano_arena.h"
#include "cliente/modelo/infraestructura.h"
#include "cliente/modelo/terreno.h"
#include "cliente/video/camara.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Juego {
public:
    /**
     * \brief Inicializa el modelo de juego.
     * 
     * id_jugador_actual es el identificador del servidor del jugador actual.
     */
    Juego(int id_jugador_actual_, 
        const nlohmann::json& mapa,
        const nlohmann::json& edificios,
        const nlohmann::json& ejercito);

    /**
     * \brief Devuelve true si el juego terminó.
     */
    bool esta_terminado() const;

    /**
     * \brief Renderiza el juego.
     * 
     */
    void renderizar(Ventana& ventana, Camara& camara);

    /**
     * \brief Actualiza el modelo del juego.
     * 
     * Avanza el estado del juego al instante t_ms.
     * 
     * t_ms es el tiempo en milisegundos desde que se inició el juego al 
     * momento a actualizar. Este valor debe ser siempre mayor o igual al valor
     * utilizado la última vez que se llamó a este método.
     */
    void actualizar(int t_ms);

    /**
     * \brief Fuerza la finalización del juego.
     */
    void detener();

    /**
     * \brief Obtiene el dinero que el jugador posee actualmente.
     */
    int obtener_dinero() const;

    /**
     * \brief Obtiene la energia que el jugador posee actualmente.
     */
    int obtener_energia() const;

    /**
     * \brief Obtiene la posición central del jugador.
     * 
     * La posición central es el centro de construcciones.
     */
    Posicion obtener_centro();

    /**
     * \brief Devuelve el terreno del juego.
     */
    Terreno& obtener_terreno();

    /**
     * \brief Devuelve el administrador de edificios del juego.
     */
    Infraestructura& obtener_infraestructura();
    
    /**
     * \brief Devuelve el administrador de tropas del juego.
     */
    Ejercito& obtener_ejercito();


    /***** Eventos recibidos desde el servidor *****/

    /**
     * \brief Actualiza el dinero del jugador actual
     */
    void actualizar_dinero(int nuevo_dinero, int nuevo_maximo);

    /**
     * \brief Actualiza la energía del jugador actual
     */
    void actualizar_energia(int nueva_energia, int nuevo_maximo);
    
    /**
     * \brief Hace aparecer al gusano de arena en la posición (x, y).
     */
    void mostrar_gusano(int x, int y);

    /**
     * \brief Destructor.
     */
    ~Juego();

private:
    Terreno terreno;
    GusanoArena gusano;
    Infraestructura infraestructura;
    Ejercito ejercito;
    bool esta_jugando;
    int dinero = 0, energia = 0;
    int id_jugador_actual;
};

} // namespace cliente

#endif // _JUEGO_H_
