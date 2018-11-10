#ifndef _JUEGO_H_
#define _JUEGO_H_

#include <string>
#include <vector>

#include "cliente/modelo/gusano_arena.h"
#include "cliente/modelo/jugador.h"
#include "cliente/modelo/terreno.h"
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
     * Procesa las actualizaciones obtenidas desde el servidor.
     */
    void renderizar(Ventana& ventana);

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

    void iniciar_construccion(const std::string& clase);

    /**
     * \brief Construye un nuevo edificio en (x, y).
     * 
     * Construye un nuevo edificio con el identificador indicado, de la
     * clase especificada en la posición (x, y), medida en celdas.
     * 
     * Si ya hay un edificio con dicho identificador [TODO] no hace nada.
     */
    void construir_edificio(int id, const std::string& clase, int x, int y);
    
    /**
     * \brief Destruye el edificio con ID indicado.
     */
    void destruir_edificio(int id);

    /**
     * \brief Crea una nueva tropa.
     */
    void crear_tropa(int id, const std::string& clase, int x, int y);

    /**
     * \brief Elimina una tropa.
     */
    void destruir_tropa(int id);

    /**
     * \brief Hace aparecer al gusano de arena en la posición (x, y).
     */
    void mostrar_gusano(int x, int y);

    /**
     * \brief Fuerza la finalización del juego.
     */
    void detener();

    /**
     * \brief Devuelve los edificios que existen en el juego.
     */
    std::vector<const Edificio*> obtener_edificios() const;

    void seleccionar_tropas(int x0, int y0, int x1, int y1);
    
    void seleccionar_edificio(int x, int y);

    void sincronizar_tropa(int id_tropa, int x, int y);

    void indicar_camino_tropa(int id_tropa, 
        const std::vector<std::pair<int, int>>& camino);
    
    /**
     * \brief Mueve la cámara incrementando la posición en (dx, dy).
     * 
     * Si la cámara está en uno de sus límites esta operación no tiene efecto.
     * 
     * (dx, dy) es el desplazamiento en celdas.
     */
    void mover_camara(int dx, int dy);

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
