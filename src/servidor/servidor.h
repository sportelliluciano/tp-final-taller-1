#ifndef _SERVIDOR_H_
#define _SERVIDOR_H_

#include <list>

#include "servidor/admin_mapas.h"
#include "servidor/cliente.h"
#include "servidor/lobby.h"
#include "servidor/socket_aceptador.h"
#include "servidor/procesador_configuracion.h"

namespace servidor {

class Servidor {
public:
    /**
     * \brief Inicializa un nuevo servidor.
     */
    Servidor(const ProcesadorConfiguracion& configuracion);

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
    Lobby lobby;
    SocketAceptador aceptador;
    
    std::list<Cliente> clientes;
    bool terminar = false;

    /**
     * \brief Elimina y libera los recursos asociados a los clientes que
     *        se desconectaron.
     */
    void limpiar_clientes();
};

} // namespace servidor

#endif // _SERVIDOR_H_
