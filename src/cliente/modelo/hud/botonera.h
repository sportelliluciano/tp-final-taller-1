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
    /**
     * \brief Crea una nueva botonera con el tamaño indicado.
     */
    Botonera(int ancho_, int alto_);

    /**
     * \brief Agrega un nuevo widget a la botonera.
     */
    void agregar_widget(Widget& widget);

    /** Interfaz widget **/
    int obtener_alto() const override;
    int obtener_ancho() const override;
    void renderizar(Ventana& ventana, const Posicion& punto) override;

protected:
    /** Métodos sobreescritos de contendor. **/
    std::vector<Widget*> obtener_widgets();
    std::vector<std::pair<Posicion, Widget*>>
        obtener_widgets(const Posicion& punto);

private:
    std::list<Widget*> widgets;
    int ancho, alto, padding_x, padding_y, spacing;
    
    /**
     * \brief Calcula el padding requerido para centrar la botonera en base 
     *        al tamaño y la cantidad de los botones.
     */
    void calcular_padding();
};

} // namespace cliente

#endif // _BOTONERA_H_
