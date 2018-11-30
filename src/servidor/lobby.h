#ifndef _LOBBY_H_
#define _LOBBY_H_

#include <thread>
#include <mutex>
#include <unordered_set>
#include <unordered_map>

#include "servidor/admin_mapas.h"
#include "servidor/cliente.h"
#include "servidor/sala.h"
#include "servidor/procesador_configuracion.h"

namespace servidor {

/**
 * \brief El lobby es la sala donde están los clientes que se conectaron al
 *        servidor pero no se unieron a ninguna sala.
 */
class Lobby {
public:
    Lobby(const ProcesadorConfiguracion& configuracion);

    /**
     * \brief Agrega un nuevo cliente al lobby.
     */
    void agregar_cliente(Cliente& cliente);

    /**
     * \brief Notifica al lobby la desconexión de un cliente.
     * 
     * El lobby eliminará el cliente del mismo, con lo cual es seguro eliminar
     * el cliente luego de llamar a este método.
     */
    void notificar_desconexion(Cliente& cliente);

    /**
     * \brief Verifica las salas existentes y realiza una limpieza eliminado
     *        aquellas que no tienen jugadores conectados.
     */
    void limpiar_salas();

    /**
     * \brief Detiene todas las partidas en curso y cierra el lobby.
     */
    void detener_todo();

private:
    AdminMapas administrador_mapas;
    const nlohmann::json& data_edificios;
    const nlohmann::json& data_ejercitos;

    /**
     * \brief Indica si se llamó al método detener_todo.
     */
    bool lobby_cerrado = false;

    /**
     * \brief Protección de la concurrencia para el lobby.
     */
    std::mutex m_lobby;

    /**
     * \brief Clientes conectados al lobby.
     */
    std::unordered_set<Cliente*> clientes;

    /**
     * \brief Salas creadas.
     */
    std::unordered_map<std::string, Sala> salas;

    /**
     * \brief A qué sala está conectado cada cliente.
     */
    std::unordered_map<Cliente*, Sala*> salas_clientes;
    
    /**
     * \brief Procesa un evento proveniente de un cliente.
     * 
     * Este método se ejecuta de forma asincrónica desde el hilo de recepción
     * de cualquier cliente.
     */
    void procesar_evento(Cliente& cliente, const nlohmann::json& dato);

    /**
     * Los siguientes métodos responden a los distintos mensajes de los 
     * clientes.
     */

    /**
     * \brief Lista las salas disponibles en el servidor.
     */
    void listar_salas(Cliente& cliente);

    /**
     * \brief Lista los mapas disponibles para crear una sala.
     */
    void listar_mapas(Cliente& cliente);

    /**
     * \brief Une al cliente a la sala indicada.
     */
    bool unirse(Cliente& cliente, const std::string& nombre_sala);

    /**
     * \brief Deja la sala.
     */
    bool dejar_sala(Cliente& cliente);

    /**
     * \brief Crea una nueva sala con el nombre y mapa indicado.
     */
    bool crear_sala(Cliente& cliente, const std::string& nombre, 
        const std::string& mapa);
    
    /**
     * \brief Inicia el juego.
     */
    bool iniciar_juego(Cliente& cliente);
};

} // namespace servidor

#endif // _LOBBY_H_
