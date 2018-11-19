#ifndef _I_JUGADOR_H_
#define _I_JUGADOR_H_

#include <string>
#include <vector>
#include <utility>

#include "libs/json.hpp"

/**
 * \brief Interfaz Jugador. Permite notificar al jugador los eventos 
 *        precedentes del modelo.
 * 
 * Esta interfaz define los eventos que el modelo puede enviar a cada jugador.
 */
class IJugador {
public:
    /**
     * \brief Obtiene el ID del jugador.
     * 
     * Este ID es único.
     */
    virtual int obtener_id() const = 0;

    /**
     * \brief Devuelve la casa a la que pertenece el jugador.
     */
    virtual const std::string& obtener_casa() const = 0;

    /**
     * \brief Devuelve el nombre del jugador.
     */
    virtual const std::string& obtener_nombre() const = 0;
    
    /**
     * \brief Envia el mapa al cliente.
     */
    virtual void enviar_mapa(const nlohmann::json& mapa) = 0;
    
    /**
     * \brief Envia los datos sobre la infraestructura al cliente.
     */
    virtual void enviar_infraestructura(
        const nlohmann::json& infraestructura) = 0;
    
    /**
     * \brief Envia los datos sobre el ejército al cliente.
     */    
    virtual void enviar_ejercito(const nlohmann::json& ejercito) = 0;

    /**
     * \brief Indica al jugador que inicie la construcción de un edificio
     *        de la clase indicada.
     * 
     * clase: Clase del edificio a construir
     * ms: Tiempo de la construcción en milisegundos
     */
    virtual void iniciar_construccion(const std::string& clase, int ms) = 0;

    /**
     * \brief Sincroniza el temporizador de construcción de la clase con el
     *        del jugador.
     * 
     * clase: Clase del edificio a construir
     * ms: Tiempo a sincronizar, en milisegundos
     */
    virtual void sincronizar_construccion(const std::string& clase, int ms) = 0;

    /**
     * \brief Indica al jugador el factor de escala de la velocidad de 
     *        construcción.
     * 
     * factor: 1.0 indica velocidad normal, 2.0 doble veloc, 0.5 media veloc
     */
    virtual void set_velocidad_cc(float factor) = 0;

    /**
     * \brief Actualiza la cola de construcciones de la clase indicada del 
     *        jugador.
     * 
     * clase: Clase de edificio a modificar la cola
     * cantidad: Cantidad de edificios encolados.
     */
    virtual void actualizar_cola_cc(const std::string& clase, int cantidad) = 0;

    /**
     * \brief Realiza daño a un edificio. No es necesario que el edificio 
     *        atacado pertenezca al jugador.
     * 
     * id_edificio: ID del edificio atacado
     * nueva_vida: Vida del edificio después del ataque.
     */
    virtual void atacar_edificio(int id_edificio, int nueva_vida) = 0;

    /**
     * \brief Crea un nuevo edificio mostrando la animación de su construcción.
     * 
     * id_edificio: ID del nuevo edificio
     * clase: Clase del edificio a crear
     * celda_x, celda_y: Posición del edificio
     * id_propietario: ID del jugador propietario del edificio
     */
    virtual void crear_edificio(int id_edificio, const std::string& clase, 
        int celda_x, int celda_y, int id_propietario) = 0;

    /**
     * \brief Destruye el edificio indicado mostrando la animación de su 
     *        destrucción.
     * 
     * id_edificio: ID del edificio a destruir.
     */
    virtual void destruir_edificio(int id_edificio) = 0;

    /**
     * \brief Agrega un nuevo edificio al terreno sin animar su construcción.
     * 
     * id_edifcio: ID del edificio a agregar
     * clase: Clase del edificio a agregar
     * celda_x, celda_y: Posición del edificio
     * id_propietario: ID del jugador propietario del edificio
     * vida: Vida actual del edificio
     */
    virtual void agregar_edificio(int id_edificio, const std::string& clase,
        int celda_x, int celda_y, int id_propietario, int vida) = 0;

    /**
     * \brief Elimina un edificio del juego sin animar su destrucción
     * 
     * id_edificio: ID del edificio a eliminar
     */
    virtual void eliminar_edificio(int id_edificio) = 0;

    /**** Métodos de ejército ****/
    
    /**
     * \brief Inicia el entrenamiento de una tropa de la clase indicada.
     * 
     * clase: Clase de la tropa a entrenar
     */
    virtual void iniciar_entrenamiento(const std::string& clase, int ms) = 0;

    /**
     * \brief Sincroniza el tiempo de entrenamiento con el jugador.
     * 
     * clase: Clase de la tropa a sincronizar
     * ms: Tiempo del modelo, en milisegundos
     */
    virtual void sincronizar_entrenamiento(const std::string& clase, int ms) = 0;

    /**
     * \brief Actualiza la cola de entrenamientos del jugador.
     * 
     * clase: Clase de tropa a actualizar la cola
     * cantidad: Cantidad nueva de entrenamientos pendientes
     */
    virtual void actualizar_cola_ee(const std::string& clase, int cantidad) = 0;

    /**
     * \brief Crea una nueva tropa.
     * 
     * id_tropa: ID único de la tropa creada.
     * clase: Clase de la tropa
     * pos_x, pos_y: Posición de la tropa en (TODO).
     * vida: Vida actual de la tropa
     * id_propietario: ID del jugador dueño de la tropa
     */
    virtual void crear_tropa(int id_tropa, const std::string& clase, 
        int pos_x, int pos_y, int vida, int id_propietario) = 0;

    /**
     * \brief Indica que la tropa comenzará a seguir un camino
     * 
     * id_tropa: Tropa a mover
     * camino: Puntos que la tropa debe seguir. 
     * 
     * Se espera que el cliente interpole linealmente la posición para caminar
     * entre un punto y el otro. El cliente debería esperar en cada punto a
     * que se emita un evento `sincronizar_tropa`.
     */
    virtual void mover_tropa(int id_tropa, 
        const std::vector<std::pair<int, int>>& camino) = 0;

    /**
     * \brief Sincroniza la posición de una tropa del jugador con su posición
     *        en el servidor.
     * 
     * id_tropa: ID de tropa a sincronizar
     * pos_x, pos_y: Posición de la tropa en el servidor
     * 
     * Este evento permite sincronizar el camino que sigue la tropa con la 
     * interpolación realizada en el cliente.
     */
    virtual void sincronizar_tropa(int id_tropa, int pos_x, int pos_y) = 0;

    /**
     * \brief Realiza daño sobre la tropa indicada.
     * 
     * id_tropa: ID de la tropa a dañar
     * nueva_vida: Vida de la tropa luego de aplicar el daño
     */
    virtual void atacar_tropa(int id_tropa, int nueva_vida) = 0;

    /**
     * \brief Elimina una tropa del juego
     * 
     * id_tropa: ID de la tropa a ser destruida
     */
    virtual void destruir_tropa(int id_tropa) = 0;

    /**** Disparos. ****/
    
    /**
     * TODO:
     * 
     * Estos eventos todavía no están definidos.
     */
    virtual void lanzar_misil() = 0;
    virtual void onda_sonido() = 0;
    virtual void lanzar_plasma() = 0;
    virtual void sincronizar_disparo() = 0;

    
    /**
     * \brief Indica al jugador que la cosechadora empezó a descargar especia.
     * 
     * id_tropa: ID de la cosechadora que está descargando especia.
     */
    virtual void cosechadora_descargar(int id_tropa) = 0;

    
    /**
     * \brief Indica al jugador que la posición indicada en el terreno ya no
     * tiene más especia.
     */
    virtual void eliminar_especia() = 0;

    /**** Eventos generales. ****/
    
    /**
     * \brief Indica al jugador que debe mostrar el gusano de arena en la 
     *        posición indicada.
     */
    virtual void mostrar_gusano(int celda_x, int celda_y) = 0;

    /**
     * \brief Actualiza el dinero del jugador.
     * 
     * nuevo_dinero: Dinero actual del jugador.
     * nuevo_maximo: Máximo de dinero que el jugador puede tener actualmente.
     */
    virtual void actualizar_dinero(int nuevo_dinero, int nuevo_maximo) = 0;

    /**
     * \brief Actualiza la energía del jugador.
     * 
     * nueva_energia: Energía actual del jugador.
     * nuevo_maximo: Máximo de energía actual del jugador.
     */
    virtual void actualizar_energia(int nueva_energia, int nuevo_maximo) = 0;

    /**
     * \brief Eventos misceláneos.
     */

    /**
     * \brief Indica al jugador que el juego está iniciando y su ID de jugador.
     */
    virtual void juego_iniciando() = 0;

    /**
     * \brief Indica al jugador que se agregó un nuevo jugador.
     * 
     * El jugador agregado estará por defecto en "espera", es decir, 
     * inicializando el juego hasta que no envíe un evento jugador_listo.
     */
    virtual void crear_jugador(int id, const std::string& nombre, 
        const std::string& casa) = 0;
    
    /**
     * \brief Indica al jugador que el jugador con el id indicado ya está
     *        listo para iniciar la partida.
     */
    virtual void jugador_listo(int id) = 0;

    /**
     * \brief Indica al cliente que el juego terminó.
     * 
     * Luego de enviarse este evento se cerrará la conexión.
     */
    virtual void juego_terminado(int id_ganador) = 0;

    /**
     * \brief Destructor virtual.
     */
    virtual ~IJugador() { }
};

#endif 
