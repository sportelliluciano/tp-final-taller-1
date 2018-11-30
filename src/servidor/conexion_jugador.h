#ifndef _CONEXION_JUGADOR_H_
#define _CONEXION_JUGADOR_H_

#include "comun/conexion.h"
#include "comun/eventos_cliente.h"
#include "comun/i_jugador.h"
#include "comun/i_modelo.h"
#include "servidor/cliente.h"

namespace servidor {



class ConexionJugador : public IJugador {
public:
    ConexionJugador(Cliente& cliente, int id_);

    /**
     * \brief Obtiene el ID del jugador.
     * 
     * Este ID es único.
     */
    int obtener_id() const;

    /**
     * \brief Devuelve la casa a la que pertenece el jugador.
     */
    virtual const std::string& obtener_casa() const;
    
    /**
     * \brief Devuelve el nombre del jugador.
     */
    virtual const std::string& obtener_nombre() const;

    /**
     * \brief Envia los datos de inicialización al cliente
     */
    virtual void inicializar(
        int id,
        const nlohmann::json& mapa,
        const nlohmann::json& infraestructura,
        const nlohmann::json& ejercito);

    /**
     * \brief Indica al jugador que el juego está iniciando y le comunica
     *        su ID de jugador.
     */
    virtual void juego_iniciando();

    /**
     * \brief Indica al jugador que inicie la construcción de un edificio
     *        de la clase indicada.
     * 
     * clase: Clase del edificio a construir
     * ms: Tiempo de la construcción en milisegundos
     */
    void iniciar_construccion(const std::string& clase, int ms);

    /**
     * \brief Sincroniza el temporizador de construcción de la clase con el
     *        del jugador.
     * 
     * clase: Clase del edificio a construir
     * ms: Tiempo a sincronizar, en milisegundos
     */
    void sincronizar_construccion(const std::string& clase, int ms);

    /**
     * \brief Indica al jugador el factor de escala de la velocidad de 
     *        construcción.
     * 
     * factor: 1.0 indica velocidad normal, 2.0 doble veloc, 0.5 media veloc
     */
    void set_velocidad_cc(float factor);

    /**
     * \brief Actualiza la cola de construcciones de la clase indicada del 
     *        jugador.
     * 
     * clase: Clase de edificio a modificar la cola
     * cantidad: Cantidad de edificios encolados.
     */
    void actualizar_cola_cc(const std::string& clase, int cantidad);

    /**
     * \brief Crea un nuevo edificio mostrando la animación de su construcción.
     * 
     * id_edificio: ID del nuevo edificio
     * clase: Clase del edificio a crear
     * celda_x, celda_y: Posición del edificio
     * id_propietario: ID del jugador propietario del edificio
     */
    void crear_edificio(int id_edificio, const std::string& clase, 
        int celda_x, int celda_y, int id_propietario);

    /**
     * \brief Destruye el edificio indicado mostrando la animación de su 
     *        destrucción.
     * 
     * id_edificio: ID del edificio a destruir.
     */
    void destruir_edificio(int id_edificio);

    /**
     * \brief Agrega un nuevo edificio al terreno sin animar su construcción.
     * 
     * id_edifcio: ID del edificio a agregar
     * clase: Clase del edificio a agregar
     * celda_x, celda_y: Posición del edificio
     * id_propietario: ID del jugador propietario del edificio
     * vida: Vida actual del edificio
     */
    void agregar_edificio(int id_edificio, const std::string& clase,
        int celda_x, int celda_y, int id_propietario, int vida);

    /**
     * \brief Elimina un edificio del juego sin animar su destrucción
     * 
     * id_edificio: ID del edificio a eliminar
     */
    void eliminar_edificio(int id_edificio);

    /**** Métodos de ejército ****/
    
    /**
     * \brief Inicia el entrenamiento de una tropa de la clase indicada.
     * 
     * clase: Clase de la tropa a entrenar
     */
    void iniciar_entrenamiento(const std::string& clase, int ms);

    /**
     * \brief Sincroniza el tiempo de entrenamiento con el jugador.
     * 
     * clase: Clase de la tropa a sincronizar
     * ms: Tiempo del modelo, en milisegundos
     */
    void sincronizar_entrenamiento(const std::string& clase, int ms);

    /**
     * \brief Actualiza la cola de entrenamientos del jugador.
     * 
     * clase: Clase de tropa a actualizar la cola
     * cantidad: Cantidad nueva de entrenamientos pendientes
     */
    void actualizar_cola_ee(const std::string& clase, int cantidad);

    /**
     * \brief Crea una nueva tropa.
     * 
     * id_tropa: ID único de la tropa creada.
     * clase: Clase de la tropa
     * pos_x, pos_y: Posición de la tropa en (TODO).
     * vida: Vida actual de la tropa
     * id_propietario: ID del jugador dueño de la tropa
     */
    void crear_tropa(int id_tropa, const std::string& clase, 
        int pos_x, int pos_y, int vida, int id_propietario);

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
    void mover_tropa(int id_tropa, 
        const std::vector<std::pair<int, int>>& camino);

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
    void sincronizar_tropa(int id_tropa, int pos_x, int pos_y);

    /**
     * \brief Realiza daño sobre la tropa indicada.
     * 
     * id_atacante: ID de la tropa que está atacando
     * id_victima: ID de la tropa o edificio a dañar
     * nueva_vida: Vida de la víctima luego de aplicar el daño
     */
    void atacar(int id_atacante, int id_victima, int nueva_vida);

    /**
     * \brief Elimina una tropa del juego
     * 
     * id_tropa: ID de la tropa a ser destruida
     */
    void destruir_tropa(int id_tropa);
    
    /**
     * \brief Indica al jugador que la cosechadora empezó a descargar especia.
     * 
     * id_tropa: ID de la cosechadora que está descargando especia.
     */
    void cosechadora_descargar(int id_tropa);

    
    /**
     * \brief Indica al jugador que la posición indicada en el terreno ya no
     * tiene más especia.
     */
    void eliminar_especia(int celda_x, int celda_y);

    /**** Eventos generales. ****/
    
    /**
     * \brief Indica al jugador que debe mostrar el gusano de arena en la 
     *        posición indicada.
     */
    void mostrar_gusano(int celda_x, int celda_y);

    /**
     * \brief Actualiza el dinero del jugador.
     * 
     * nuevo_dinero: Dinero actual del jugador.
     * nuevo_maximo: Máximo de dinero que el jugador puede tener actualmente.
     */
    void actualizar_dinero(int nuevo_dinero, int nuevo_maximo);

    /**
     * \brief Actualiza la energía del jugador.
     * 
     * nueva_energia: Energía actual del jugador.
     * nuevo_maximo: Máximo de energía actual del jugador.
     */
    void actualizar_energia(int nueva_energia, int nuevo_maximo);

    /**
     * \brief Eventos misceláneos.
     */

    /**
     * \brief Indica al jugador que se agregó un nuevo jugador.
     * 
     * El jugador agregado estará por defecto en "espera", es decir, 
     * inicializando el juego hasta que no envíe un evento jugador_listo.
     */
    virtual void crear_jugador(int id_jugador, const std::string& nombre_, 
        const std::string& casa_);
    
    /**
     * \brief Indica al jugador que el jugador con el id indicado ya está
     *        listo para iniciar la partida.
     */
    virtual void jugador_listo(int id_jugador);

    /**
     * \brief Indica al cliente que el juego terminó.
     * 
     * Luego de enviarse este evento se cerrará la conexión.
     */
    virtual void juego_terminado(int id_ganador);

    void set_estado(bool listo_);
    bool esta_listo() const;

    virtual ~ConexionJugador();

private:
    Cliente& conexion_cliente;
    int id;
    std::string casa = "ordos", nombre = "Jugador";
    bool listo = false;

    /**
     * \brief Envia al jugador la información indicada.
     */
    void notificar(const nlohmann::json& data);
};

}

#endif // _CONEXION_JUGADOR_H_
