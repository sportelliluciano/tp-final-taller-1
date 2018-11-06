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
    Juego(const char *edificio);

    /**
     * \brief Devuelve true si el juego terminó.
     */
    bool esta_terminado() const;

    /**
     * \brief Actualiza el juego.
     * 
     * Procesa las actualizaciones obtenidas desde el servidor.
     */
    void renderizar(Ventana& ventana);

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
