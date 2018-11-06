#ifndef _BOTONERA_H_
#define _BOTONERA_H_

#include <list>

#include "cliente/modelo/hud/boton.h"
#include "cliente/video/rectangulo.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Contenedor de botones movible.
 */
class Botonera {
public:
    Botonera(int ancho_, int alto_);

    void agregar_boton(const Boton& btn, int x, int y);

    Boton* crear_boton();

    /**
     * \brief Renderiza una sección de la botonera.
     */
    void renderizar(Ventana& ventana, int x, int y);

    /**
     * \brief Manejador para el click del mouse.
     * 
     * Este método es llamado cuando se hace click en el área de la botonera, 
     * pasándole como parámetro la posición donde se hizo click, *relativa* a la
     * botonera.
     */
    void click_derecho(int x, int y);
    void click_izquierdo(int x, int y);

    /**
     * \brief Manejador para el hover del mouse.
     * 
     * Este método es llamado cuando el mouse pasa por encima de la botonera,
     * pasándole como parámetro la posición donde está el mouse, *relativa* a
     * la botonera.
     */
    void hover_in(int x, int y);
    void hover_out(int x, int y);

private:
    std::list<Boton> botones;
    int ancho, alto, padding_x, padding_y, spacing;
    void calcular_padding();
};

} // namespace cliente

#endif // _BOTONERA_H_
