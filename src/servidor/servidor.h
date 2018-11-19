#ifndef _SERVIDOR_H_
#define _SERVIDOR_H_

#include <list>

#include "servidor/cliente.h"
#include "servidor/lobby.h"
#include "servidor/socket_aceptador.h"

namespace servidor {

class Servidor {
public:
    /**
     * \brief Inicializa un nuevo servidor que escuchará en el servicio 
     *        indicado.
     */
    Servidor(const std::string& servicio);

    /**
     * \brief Inicia el servidor. Este método bloqueará durante toda la 
     *        ejecución del servidor.
     */
    void correr();

    /**
     * \brief Detiene a todos los clientes y al servidor.
     * 
     * Si se inició el servidor, este método debe ser llamado al finalizarlo.
     */
    void detener();

    /**
     * \brief Destructor.
     */
    ~Servidor();

private:
    SocketAceptador aceptador;
    std::list<Cliente> clientes;
    Lobby lobby;
    bool terminar = false;

    /**
     * \brief Elimina y libera los recursos asociados a los clientes que
     *        se desconectaron.
     */
    void limpiar_clientes();
};

} // namespace servidor

#endif // _SERVIDOR_H_
