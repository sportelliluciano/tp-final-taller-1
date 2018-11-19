#ifndef _BOTONERA_H_
#define _BOTONERA_H_

#include <list>

#include "cliente/modelo/hud/boton_construccion.h"
#include "cliente/video/rectangulo.h"
#include "cliente/video/ventana.h"
#include "cliente/video/widgets/contenedor.h"

namespace cliente {

/**
 * \brief Contenedor de botones movible.
 */
class Botonera : public Contenedor {
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
    void renderizar(Ventana& ventana, const Posicion& punto) override;


    /**
     * \brief Agrega un nuevo widget a la botonera.
     */
    void agregar_widget(Widget& widget);

protected:
    virtual std::vector<Widget*> obtener_widgets();

    virtual std::vector<std::pair<Posicion, Widget*>>
        obtener_widgets(const Posicion& punto);

private:
    std::list<Widget*> widgets;
    int ancho, alto, padding_x, padding_y, spacing;
    void calcular_padding();
};

} // namespace cliente

#endif // _BOTONERA_H_
