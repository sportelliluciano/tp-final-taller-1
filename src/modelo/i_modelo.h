#ifndef _I_MODELO_H_
#define _I_MODELO_H_

#include <string>

#include "i_jugador.h"

/**
 * \brief Interfaz servidor-modelo
 */
class IModelo {
public:
    /**
     * \brief Inicia la partida
     */

    virtual void iniciar_partida() = 0;

    /**
     * \brief Devuelve true si la partida ya fue iniciada
     */
    virtual bool partida_iniciada() const = 0;

    /**
     * \brief Indica si el juego terminó
     */
    virtual bool juego_terminado() const = 0;

    /**
     * \brief Actualiza un paso del modelo.
     * 
     * Actualiza un paso de `dt_ms` milisegundos en el modelo.
     * El parámetro `dt_ms` indica cuántos milisegundos pasaron entre 
     * la última vez que se llamó al método actualizar y este llamado.
     * Se garantiza que `dt_ms > 0`.
     */
    virtual void actualizar(int dt_ms) = 0;

    /**
     * \brief Crea un nuevo jugador en el modelo con el ID y la casa indicada.
     * 
     * El nuevo jugador creado será agregado al juego en la casa indicada.
     * 
     * Si ya existe un jugador con dicho ID se lanzará una excepción.
     */
    virtual void crear_jugador(IJugador* jugador) = 0;

    /**
     * \brief Indica al modelo que el jugador se desconectó.
     */
    virtual void jugador_desconectado(IJugador* jugador) = 0;

    /**
     * \brief Iniciar la construcción de un edificio.
     * 
     * El jugador solicitó el inicio de la construcción de un edificio
     * de la clase indicada. El método devuelve true si la construcción
     * fue iniciada o false en caso contrario (falta de fondos, deshabilitado).
     */
    virtual bool iniciar_construccion_edificio(IJugador* jugador,
        const std::string& clase) = 0;
    
    /**
     * \brief Cancela la construcción de un edificio.
     * 
     * El jugador solicitó cancelar la construcción de un edificio
     * de la clase indicada. El método devuelve true si se cancelo la 
     * construccion o false en caso contrario (no había que cancelar).
     */
    virtual bool cancelar_construccion_edificio(IJugador* jugador, 
        const std::string& clase) = 0;
    
    /**
     * \brief Ubica un edificio construido en el mapa.
     */
    virtual bool ubicar_edificio(IJugador* jugador, int celda_x, int celda_y,
        const std::string& clase) = 0;

    /**
     * \brief Vende un edificio del jugador.
     * 
     * El jugador solicitó vender el edificio indicado.
     * 
     * El método devuelve false si se produjo algún error lógico que impida la
     * venta del edificio.
     */
    virtual bool vender_edificio(IJugador* jugador, int id_edificio) = 0;

    /**
     * \brief Inicia el entrenamiento de una tropa de la clase indicada.
     * 
     * Inicia el entrenamiento de una tropa indicada del jugador. 
     * 
     * El método devuelve false si no se puede iniciar el entrenamiento.
     */
    virtual bool iniciar_entrenamiento_tropa(IJugador* jugador,
        const std::string& clase) = 0;

    /**
     * \brief Cancela el entrenamiento de una tropa de la clase indicada.
     * 
     * El método devuelve false si no se puede cancelar el entrenamiento (no 
     * hay nada que cancelar).
     */
    virtual bool cancelar_entrenamiento_tropa(IJugador* jugador,
        const std::string& clase) = 0;

    /**
     * \brief Inicia el movimiento de las tropas del jugador 
     *        a la posición (x_px, y_px), dada en píxeles globales.
     * 
     * El método devuelve false si no se puede mover a la posición indicada.
     */
    virtual bool mover_tropas(IJugador* jugador, const std::vector<int>& ids,
        int x_px, int y_px) = 0;

    /**
     * \brief El jugador quiere atacar con las tropas dadas por el ID a la
     *        tropa con id=id_atacado.
     * 
     * El método devuelve false si no se puede atacar (ej friendly-fire)
     */
    virtual bool atacar_tropa(IJugador* jugador, 
        const std::vector<int>& ids_atacantes, int id_atacado) = 0;

    /**
     * \brief El jugador quiere indicarle a la(s) cosechadora(s) que vayan
     *        a recolectar especia a la celda indicada.
     */
    virtual bool indicar_especia_cosechadora(IJugador* jugador,
        const std::vector<int>& ids, int celda_x, int celda_y) = 0;

};

#endif // _I_MODELO_H_
