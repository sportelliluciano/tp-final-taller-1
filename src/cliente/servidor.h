#ifndef _SERVIDOR_H_
#define _SERVIDOR_H_

#include <list>
#include <thread>
#include <mutex>

#include "conexion/conexion.h"
#include "conexion/evento.h"

namespace cliente {

/**
 * \brief Conexión al servidor del juego.
 * 
 * Esta clase representa la conexión con el servidor de juego y se encarga de
 * actualizar el modelo según el servidor le informe.
 */
class Servidor {
public:
    Servidor(int argc, char *argv[]);
    
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
    conexion::Evento* pop_evento();

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
    std::list<conexion::Evento*> cola_eventos;
    
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
    void push_evento(conexion::Evento* evento);
};

} // namespace cliente

#endif // _SERVIDOR_H_
