#ifndef _JUEGO_H_
#define _JUEGO_H_

#include <string>

#include "cliente/modelo/jugador.h"
#include "cliente/modelo/terreno.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Juego {
public:
    Juego(const char *edificio);

    /**
     * \brief Devuelve true si el juego terminó.
     */
    bool esta_terminado() const;

    /**
     * \brief Actualiza el juego.
     * 
     * Procesa las actualizaciones obtenidas desde el servidor.
     */
    void renderizar(Ventana& ventana);


    void construir_edificio(const std::string& clase, int x, int y);

    /**
     * \brief Fuerza la finalización del juego.
     */
    void detener();

    /**
     * \brief Destructor.
     */
    ~Juego();

private:
    Terreno *terreno;
    Jugador *jugador;
    bool esta_jugando = true;
};

} // namespace cliente

#endif // _JUEGO_H_
