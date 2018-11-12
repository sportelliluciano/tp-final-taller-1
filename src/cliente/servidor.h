#ifndef _SERVIDOR_H_
#define _SERVIDOR_H_

#include <list>
#include <thread>
#include <mutex>

#include "conexion/conexion.h"
#include "cliente/eventos/evento.h"

namespace cliente {

/**
 * \brief Conexión al servidor del juego.
 * 
 * Esta clase representa la conexión con el servidor de juego y se encarga de
 * actualizar el modelo según el servidor le informe.
 */
class Servidor {
public:
    Servidor();
    
    /**
     * \brief Inicia el hilo servidor.
     */
    void iniciar();

    /**
     * \brief Verifica si hay nuevos eventos provenientes del servidor.
     * 
     * Devuelve true si hay eventos en la cola o false en caso contrario.
     * 
     * Este método está protegido del acceso concurrente.
     */
    bool hay_eventos() const;

    /**
     * \brief Obtiene el próximo evento a procesar.
     * 
     * Devuelve el evento más antiguo recibido desde el servidor.
     * Se cede la propiedad (ownership) a la función llamente, lo cual indica
     * que debe liberar la memoria del evento al finalizar su uso mediante
     * delete.
     * 
     * Este método está protegido del acceso concurrente.
     * 
     * Lanza runtime_error si la cola está vacía.
     */
    Evento* pop_evento();

    /**
     * \brief Solicita al servidor iniciar la construccion un edificio
     *        de la clase indicada.
     */
    void iniciar_construccion(const std::string& clase);
    
    /**
     * \brief Solicita al servidor cancelar la construccion de un edificio
     *        de la clase indicada.
     */
    void cancelar_construccion(const std::string& clase);
    
    /**
     * \brief Indica al servidor donde ubicar el nuevo edificio.
     */
    void ubicar_edificio(const std::string& clase, int celda_x, int celda_y);

    /**
     * \brief Indica al servidor que se quiere vender el edificio indicado.
     */
    void vender_edificio(int id_edificio);

    /**
     * \brief Solicita al servidor iniciar el entrenamiento de una tropa
     *        de la clase indicada.
     */
    void iniciar_entrenamiento(const std::string& clase);

    /**
     * \brief Solicita al servidor cancelar el entrenamiento de una tropa
     *        de la clase indicada.
     */
    void cancelar_entrenamiento(const std::string& clase);

    /**
     * \brief Solicita al servidor mover las tropas indicas a la posición
     *        (x_px, y_px), dada en píxeles globales.
     */
    void mover_tropas(const std::vector<int>& ids, int x_px, int y_px);

    /**
     * \brief Solicita al servidor que las tropas indicadas ataquen a la 
     *        tropa con id = id_atacado.
     */
    void atacar_tropa(const std::vector<int>& ids, int id_atacado);

    /**
     * \brief Solicita al servidor indicarle a la(s) cosechadora(s) que vayan
     *        a recolectar especia a la celda indicada.
     */
    void indicar_especia_cosechadora(const std::vector<int>& ids, int celda_x, 
        int celda_y);

    /**
     * \brief Detiene el hilo y cierra la conexión con el servidor.
     */
    void detener();

    /**
     * \brief Destructor.
     */
    ~Servidor();

private:
    std::thread hilo_receptor;
    bool terminar;

    std::string clase_edificio;

    std::mutex cola_eventos_mutex;
    std::list<Evento*> cola_eventos;
    
    conexion::Conexion* conn;

    /**
     * \brief Ciclo de recepción de datos del servidor.
     */
    void recibir();

    /**
     * \brief Agrega un evento a la cola de eventos.
     * 
     * Agrega un nuevo evento proveniente del servidor a la cola de eventos.
     * Este método está protegido del acceso concurrente.
     */
    void push_evento(Evento* evento);

    /**
     * \brief Envía el evento a través de la conexión.
     */
    void enviar_evento(const nlohmann::json& evento);
};

} // namespace cliente

#endif // _SERVIDOR_H_
