#ifndef _BOTONERA_H_
#define _BOTONERA_H_

#include <list>

#include "cliente/modelo/hud/boton_construccion.h"
#include "cliente/video/rectangulo.h"
#include "cliente/video/ventana.h"
#include "cliente/video/widgets/widget.h"

namespace cliente {

/**
 * \brief Contenedor de botones movible.
 */
class Botonera : public Widget {
public:
    Botonera(int ancho_, int alto_);

    int obtener_alto() const override;

    /**
     * \brief Devuelve el ancho del widget, en píxeles.
     */
    int obtener_ancho() const override;

    /**
     * \brief Renderiza el widget en la posición (x, y).
     * 
     * Renderiza el widget en la posición (x, y) dada en píxeles, relativa a
     * la ventana (global).
     */
    void renderizar(Ventana& ventana, int x, int y) override;


    /**
     * \brief Agrega un nuevo widget a la botonera.
     */
    void agregar_widget(Widget& widget);

    /**
     * \brief Manejador para el click del mouse.
     * 
     * Este método es llamado cuando se hace click en el área de la botonera, 
     * pasándole como parámetro la posición donde se hizo click, *relativa* a la
     * botonera.
     */
    bool mouse_click_izquierdo(int x, int y) override;
    bool mouse_click_derecho(int x, int y) override;

private:
    std::list<Widget*> widgets;
    int ancho, alto, padding_x, padding_y, spacing;
    void calcular_padding();
};

} // namespace cliente

#endif // _BOTONERA_H_
