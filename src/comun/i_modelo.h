#ifndef _I_MODELO_H_
#define _I_MODELO_H_

#include <string>
#include <unordered_set>

#include "libs/json.hpp"

#include "i_jugador.h"

/**
 * \brief Interfaz servidor-modelo
 */
class IModelo {
public:
    /**
     * \brief Inicializa el modelo con el mapa, infraestructura y ejército
     *        indicados.
     * 
     * Este método será llamado antes que cualquier otro método.
     */
    virtual void inicializar(const nlohmann::json& mapa, 
        const nlohmann::json& edificios, const nlohmann::json& ejercito) = 0;
     
    /**
     * \brief Crea un nuevo jugador en el modelo.
     * 
     * Se garantiza que:
     * - No se llamará a este método dos veces con el mismo jugador.
     * - No se llamará a este método una vez que haya iniciado la partida
     *   (vía iniciar_partida).
     */
    virtual void crear_jugador(IJugador* jugador) = 0;

    /**
     * \brief Inicia la partida y envia las configuraciones correspondientes
     *        a los jugadores.
     * 
     * Este método será llamado una única vez, antes de la primer actualización
     * del modelo. 
     * 
     * Se garantiza que en el momento en que se llame al mismo ya se habrán 
     * creado todos los jugadores vía crear_jugador (no se volverá a llamar 
     * a crear_jugador luego de iniciada la partida).
     */
    virtual void iniciar_partida() = 0;

    /**
     * \brief Pregunta al modelo si se realizó la sincronización inicial.
     * 
     * Si el modelo devuelve true significa que la sincronización inicial 
     * terminó y que se puede proceder a la primer actualización del juego.
     * 
     * Si el modelo devuelve false se volverá a llamar a este método luego de
     * un tiempo.
     * 
     * Se garantiza que una vez que el método devuelve true no volverá a ser
     * llamado.
     */
    virtual bool esperar_sincronizacion_inicial() = 0;

    /**
     * \brief Realiza una actualización del modelo.
     * 
     * Actualiza un paso de `dt_ms` milisegundos en el modelo.
     * El parámetro `dt_ms` indica cuántos milisegundos pasaron entre 
     * la última vez que se llamó al método actualizar y este llamado.
     * Se garantiza que:
     *  - `dt_ms > 0`.
     *  - El primer llamado a este método será luego que esperando_jugadores 
     *    devuelva false.
     */
    virtual void actualizar(int dt_ms) = 0;

    /**
     * \brief Devuelve true si terminó la partida.
     * 
     * Este método debe indicar el momento en que el modelo ya no enviará más
     * datos a los clientes. Una vez que este método devuelva true se cerrarán
     * las conexiones con los jugadores y **todos los jugadores creados se
     * volverán inválidos**.
     * 
     * Se garantiza que una vez que este método haya devuelto true no se 
     * volverá a llamar a ningún otro método de la interfaz.
     */
    virtual bool partida_terminada() const = 0;

    /**
     * \brief Indica al modelo que el jugador indicado se desconectó.
     * 
     * El modelo es responsable de eliminar cualquier referencia que tenga
     * al jugador ya que luego de llamarse a este método el jugador será 
     * inválido.
     */
    virtual void jugador_desconectado(IJugador* jugador) = 0;

    /*************************************************************************/

    /**
     * \brief El jugador ya recibió y procesó los datos de inicialización.
     * 
     * Este evento es enviado por el cliente cuando el mismo ya recibió todos
     * los datos de inicialización [todo lo que se envíe hasta recibir un
     * evento esperar_ack] y está listo para realizar el primer renderizado
     * de la ventana de juego.
     * 
     * El proceso de inicialización es el siguiente:
     * << inicializar(mapa, ejercito, infraestructura)
     * >> jugador_listo
     * << iniciar_juego
     */
    virtual void jugador_listo(IJugador* jugador) = 0;

    /**
     * \brief Iniciar la construcción de un edificio.
     * 
     * El jugador solicitó el inicio de la construcción de un edificio
     * de la clase indicada.
     */
    virtual void iniciar_construccion_edificio(IJugador* jugador,
        const std::string& clase) = 0;
    
    /**
     * \brief Cancela la construcción de un edificio.
     * 
     * El jugador solicitó cancelar la construcción de un edificio
     * de la clase indicada. 
     */
    virtual void cancelar_construccion_edificio(IJugador* jugador, 
        const std::string& clase) = 0;
    
    /**
     * \brief Ubica un edificio construido en el mapa.
     * 
     * El jugador indicó que quiere ubicar un edificio de la clase indicada
     * en la posición (celda_x, celda_y).
     */
    virtual void ubicar_edificio(IJugador* jugador, int celda_x, int celda_y,
        const std::string& clase) = 0;

    /**
     * \brief Vende un edificio del jugador.
     * 
     * El jugador solicitó vender el edificio indicado.
     */
    virtual void vender_edificio(IJugador* jugador, int id_edificio) = 0;

    /**
     * \brief Inicia el entrenamiento de una tropa de la clase indicada.
     * 
     * Inicia el entrenamiento de una tropa indicada del jugador. 
     */
    virtual void iniciar_entrenamiento_tropa(IJugador* jugador,
        const std::string& clase) = 0;

    /**
     * \brief Cancela el entrenamiento de una tropa de la clase indicada.
     */
    virtual void cancelar_entrenamiento_tropa(IJugador* jugador,
        const std::string& clase) = 0;

    /**
     * \brief Inicia el movimiento de las tropas del jugador 
     *        a la posición (x_px, y_px), dada en píxeles globales.
     */
    virtual void mover_tropas(IJugador* jugador, 
        const std::unordered_set<int>& ids, int x_px, int y_px) = 0;

    /**
     * \brief El jugador quiere atacar con las tropas dadas por el ID a la
     *        tropa o edificio con id=id_atacado.
     */
    virtual void atacar(IJugador* jugador, 
        const std::unordered_set<int>& ids_atacantes, int id_atacado) = 0;

    /**
     * \brief El jugador quiere indicarle a la(s) cosechadora(s) que vayan
     *        a recolectar especia a la celda indicada.
     */
    virtual void indicar_especia_cosechadora(IJugador* jugador,
        const std::unordered_set<int>& ids, int celda_x, int celda_y) = 0;

    /**
     * \brief Destructor virtual.
     */
    virtual ~IModelo() { }
};

#endif // _I_MODELO_H_
