#ifndef _CLIENTE_JUEGO_H_
#define _CLIENTE_JUEGO_H_

#include "cliente/partida.h"

namespace cliente {

/**
 * \brief Cliente del juego. 
 * 
 * Esta clase encapsula el cliente del juego. La misma se encarga de generar la
 * ventana de juego, comunicarse con el servidor y comunicar al usuario errores.
 */
class ClienteJuego {
public:
    /**
     * \brief Constructor. 
     */
    ClienteJuego();

    /**
     * \brief Punto de entrada del juego.
     * 
     * Devuelve el código de salida de la aplicación.
     * 
     * Lanzará una excepción si se produce algún error.
     */
    int correr(int argc, char *argv[]);

    /**
     * \brief Destructor.
     */
    ~ClienteJuego();

private:
    Partida partida;
    bool corriendo = false;

    /**
     * \brief Ejecuta el lanzador del juego para realizar las configuraciones
     * previas a la partida.
     * 
     * Devuelve false si el usuario cerró el lanzador sin iniciar la partida,
     * o true en caso de haberse cerrado para iniciar la partida.
     */
    bool ejecutar_lanzador(int argc, char *argv[]);

    /**
     * \brief Ejecuta el cliente del juego con la configuración obtenida 
     *        desde el lanzador.
     * 
     * Ejecuta el cliente del juego, carga la partida, inicia el juego y 
     * muestra las estadísticas al finalizar.
     * 
     * Devuelve false si la configuración del lanzador estaba incompleta, 
     * true en caso de haber terminado el juego sin errores.
     */
    bool ejecutar_juego();
};

} // namespace cliente

#endif // _CLIENTE_JUEGO_H_
