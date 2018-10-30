#ifndef _CLIENTE_JUEGO_H_
#define _CLIENTE_JUEGO_H_

#include "cliente/modelo/controlador.h"
#include "cliente/modelo/juego.h"
#include "cliente/video/ventana.h"

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
    ClienteJuego(int argc, char *argv[]);

    /**
     * \brief Ejecuta el cliente del juego.
     * 
     * Ejecuta el cliente del juego, carga la partida, inicia el juego y 
     * muestra las estadísticas al finalizar. Devuelve el valor de retorno
     * del ejecutable.
     */
    int ejecutar();

    /**
     * \brief Procesa los argumentos de línea de comandos y carga el juego en
     *        base a los mismos.
     * 
     * Devuelve true si la carga fue exitosa o false si se produjo algún
     * inconveniente.
     */
    bool cargar(int argc, char *argv[]);

    /**
     * \brief Ciclo principal del juego.
     * 
     * Ejecuta el ciclo principal del juego.
     * En caso de producirse un error irrecuperable lanzará una excepción.
     */
    void jugar();

    /**
     * \brief Muestra el ganador de la partida y las estadísticas de la misma.
     * 
     * Puede lanzar una excepción en caso de que se produzca un error
     */
    void mostrar_stats();

    ~ClienteJuego();

private:
    Ventana ventana;
    Juego juego;
    Controlador controlador;
};

} // namespace cliente

#endif // _CLIENTE_JUEGO_H_
