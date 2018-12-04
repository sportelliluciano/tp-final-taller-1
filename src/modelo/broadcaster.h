#ifndef _BROADCASTER_H_
#define _BROADCASTER_H_

#include <unordered_set>

#include "comun/i_jugador.h"

namespace modelo {

/**
 * \brief Envía un mensaje a todos los jugadores conectados al juego.
 */
class Broadcaster {
public:
    /**
     * \brief Agrega un nuevo jugador a la lista de jugadores que recibirán
     *        los mensajes.
     */
    void agregar_jugador(IJugador *nuevo_jugador);

    /**
     * \brief Elimina un jugador de la lista de jugadores que recibirán los
     *        mensajes.
     */
    void eliminar_jugador(IJugador *jugador);

    /**
     * \brief Envía un mensaje a todos los jugadores.
     * 
     * Este método ejecutará el callback mensaje una vez por cada jugador que
     * vaya a recibir el mensaje.
     * No hay ningúna garantía respecto del orden en que se envíen los mensajes.
     */
    void broadcast(std::function<void(IJugador*)> mensaje);

    IJugador* obtener_jugador(int id);
private:
    std::unordered_set<IJugador*> jugadores;
};

} // namespace modelo

#endif // _BROADCASTER_H_
