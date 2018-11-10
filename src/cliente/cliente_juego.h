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
    ClienteJuego(Servidor& servidor_juego);

    /**
     * \brief Ejecuta el cliente del juego.
     * 
     * Ejecuta el cliente del juego, carga la partida, inicia el juego y 
     * muestra las estadísticas al finalizar. Devuelve el valor de retorno
     * del ejecutable.
     */
    int ejecutar();

    ~ClienteJuego();

private:
    Ventana ventana;
    Servidor& servidor;
    Juego juego;
    Controlador controlador;
};

} // namespace cliente

#endif // _CLIENTE_JUEGO_H_
