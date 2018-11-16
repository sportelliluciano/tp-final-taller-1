#ifndef _JUEGO_H_
#define _JUEGO_H_

#include <string>
#include <vector>

#include "cliente/modelo/gusano_arena.h"
#include "cliente/modelo/jugador.h"
#include "cliente/modelo/terreno.h"
#include "cliente/video/camara.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Juego {
public:
    Juego();

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
     * \brief Devuelve la cantidad de dinero que posee el jugador.
     */
    int obtener_dinero() const;

    /**
     * \brief Setea el dinero del jugador.
     */
    void setear_dinero(int dinero);

    /**
     * \brief Hace aparecer al gusano de arena en la posición (x, y).
     */
    void mostrar_gusano(int x, int y);

    /**
     * \brief Fuerza la finalización del juego.
     */
    void detener();

    Infraestructura& obtener_infraestructura();
    Ejercito& obtener_ejercito();
    Terreno& obtener_terreno();

    /**
     * \brief Destructor.
     */
    ~Juego();

private:
    Terreno *terreno;
    Jugador *jugador;
    bool esta_jugando = true;
    GusanoArena* gusano;
};

} // namespace cliente

#endif // _JUEGO_H_
