#ifndef _JUEGO_H_
#define _JUEGO_H_

#include <string>
#include <vector>

#include "libs/json.hpp"

#include "cliente/modelo/ejercito.h"
#include "cliente/modelo/gusano_arena.h"
#include "cliente/modelo/infraestructura.h"
#include "cliente/modelo/terreno.h"
#include "cliente/video/camara.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Juego {
public:
    /**
     * \brief Inicializa el modelo de juego.
     * 
     * id_jugador_actual es el identificador del servidor del jugador actual.
     */
    Juego(const std::string& casa_jugador_actual);

    void inicializar(int id_jugador, const nlohmann::json& edificios, 
       const nlohmann::json& ejercitos, const nlohmann::json& mapa);

    void crear_jugador(int id_jugador, const std::string& nombre, 
        const std::string& casa);
    
    void indicar_jugador_listo(int id_jugador);

    void sincronizar_inicio();

    /**
     * \brief Devuelve true si el juego terminó correctamente 
     */
    bool termino_correctamente() const;

    const std::string& obtener_ganador() const;

    /**
     * \brief Devuelve true si el juego terminó.
     */
    bool esta_terminado() const;

    /**
     * \brief Renderiza el juego.
     * 
     */
    void renderizar(Ventana& ventana, Camara& camara);

    /**
     * \brief Actualiza el modelo del juego.
     * 
     * Avanza el estado del juego al instante t_ms.
     * 
     * t_ms es el tiempo en milisegundos desde que se inició el juego al 
     * momento a actualizar. Este valor debe ser siempre mayor o igual al valor
     * utilizado la última vez que se llamó a este método.
     */
    void actualizar(int t_ms);

    /**
     * \brief Fuerza la finalización del juego.
     */
    void detener();

    /**
     * \brief Obtiene el dinero que el jugador posee actualmente.
     */
    int obtener_dinero() const;

    /**
     * \brief Obtiene la energia que el jugador posee actualmente.
     */
    int obtener_energia() const;

    const std::string& obtener_casa_jugador() const;

    /**
     * \brief Obtiene la posición central del jugador.
     * 
     * La posición central es el centro de construcciones.
     */
    Posicion obtener_centro();

    /**
     * \brief Devuelve el terreno del juego.
     */
    Terreno& obtener_terreno();

    /**
     * \brief Devuelve el administrador de edificios del juego.
     */
    Infraestructura& obtener_infraestructura();
    
    /**
     * \brief Devuelve el administrador de tropas del juego.
     */
    Ejercito& obtener_ejercito();

    /**
     * \brief Devuelve true si el servidor ya indicó que se sincronizó el
     *        inicio.
     */
    bool inicio_sincronizado() const;

    /**
     * \brief Devuelve true si ya se recibió el comando de inicialización del
     *        servidor.
     */
    bool inicializacion_completa() const;

    /***** Eventos recibidos desde el servidor *****/

    /**
     * \brief Actualiza el dinero del jugador actual
     */
    void actualizar_dinero(int nuevo_dinero, int nuevo_maximo);

    /**
     * \brief Actualiza la energía del jugador actual
     */
    void actualizar_energia(int nueva_energia, int nuevo_maximo);
    
    /**
     * \brief Hace aparecer al gusano de arena en la posición (x, y).
     */
    void mostrar_gusano(int x, int y);

    /**
     * \brief Indica que terminó el juego
     */
    void terminar(const std::string& nombre_ganador);

    /**
     * \brief Destructor.
     */
    ~Juego();

private:
    Terreno* terreno;
    GusanoArena* gusano;
    Infraestructura* infraestructura;
    Ejercito* ejercito;
    bool esta_jugando;
    bool termino_ok = false;
    int dinero = 0, energia = 0;
    std::string casa;
    bool sincronizado = false, inicializado = false;
    std::string ganador = "-";
};

} // namespace cliente

#endif // _JUEGO_H_
