#ifndef _SERVIDOR_PROTEGIDO_H_
#define _SERVIDOR_PROTEGIDO_H_

#include <mutex>
#include <string>
#include <vector>

#include "libs/json.hpp"

#include "cliente/red/servidor.h"

namespace cliente {

/**
 * \brief Conexión al servidor del juego.
 * 
 * Esta clase representa la conexión con el servidor de juego y se encarga de
 * actualizar el modelo según el servidor le informe.
 */
class ServidorProtegido {
public:
    /**
     * \brief Abre la conexión al servidor.
     */
    ServidorProtegido();

    /**
     * \brief Setea el servidor a proteger de la concurrencia.
     */
    void set_servidor(Servidor* servidor_a_proteger);

    /**
     * \brief Setea la casa del jugador.
     */
    void set_casa(const std::string& casa);
    
    /**
     * \brief Setea el nombre del jugador.
     */
    void set_nombre(const std::string& nombre);
    
    /**
     * \brief Indica al servidor que el jugador ya inició el cliente.
     */
    void iniciar_juego();
    
    /**
     * \brief Obtiene el estado de los jugadores en la sala actual.
     * 
     * Se devuelve un arreglo de objetos JSON con todos los jugadores excepto
     * el actual.
     */
    std::vector<nlohmann::json>
        obtener_jugadores_sala(const std::string& sala);

    /**
     * \brief Obtiene las salas disponibles en el servidor.
     * 
     * Esta función bloquea hasta que se hayan recibido los datos.
     */
    std::vector<std::string> obtener_nombres_salas();

    /**
     * \brief Obtiene los mapas disponibles para una nueva sala.
     * 
     * Esta función bloquea hasta que se hayan recibido los datos.
     */
    std::vector<std::string> obtener_nombres_mapas();

    /**
     * \brief Une al cliente a la sala indicada.
     * 
     * Devuelve true si se pudo unir, false en caso contrario.
     */
    bool unirse_a_sala(const std::string& sala);

    /**
     * \brief Deja la sala a la que el cliente está conectado actualmente.
     * 
     * Devuelve true si se pudo dejar, false en caso contrario.
     */
    bool dejar_sala();

    /**
     * \brief Crea una nueva sala y une al jugador actual a la misma.
     *        Devuelve true si se creó la sala; false en caso contrario.
     */
    bool crear_sala(const std::string& nombre, const std::string& mapa);

private:
    std::mutex m_servidor;
    Servidor *servidor;
};

} // namespace cliente

#endif // _SERVIDOR_PROTEGIDO_H_
