#ifndef _LOBBY_H_
#define _LOBBY_H_

#include <thread>
#include <mutex>
#include <unordered_set>
#include <unordered_map>

#include "servidor/cliente.h"
#include "servidor/sala.h"

namespace servidor {

/**
 * \brief El lobby es la sala donde están los clientes que se conectaron al
 *        servidor pero no se unieron a ninguna sala.
 */
class Lobby {
public:
    Lobby() = default;

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
};

} // namespace servidor

#endif // _LOBBY_H_
