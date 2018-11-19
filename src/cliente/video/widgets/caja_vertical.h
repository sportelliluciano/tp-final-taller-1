#ifndef _CAJA_VERTICAL_H_
#define _CAJA_VERTICAL_H_

#include <functional>
#include <list>

#include "cliente/video/widgets/contenedor.h"

namespace cliente {

/**
 * \brief Caja con acomodamiento automático vertical
 * 
 * Los elementos se acomodan uno al lado del otro, verticalmente.
 */
class CajaVertical : public Contenedor {
public:
    CajaVertical(int x_, int y_, int ancho_, int alto_);

    /**
     * \brief Agrega un nuevo hijo en la parte superior del contenedor
     */
    void empaquetar_arriba(Widget& hijo);

    /**
     * \brief Agrega un nuevo hijo en la parte inferior del contenedor
     */
    void empaquetar_abajo(Widget& hijo);

    /**
     * \brief Reemplaza el widget actual por nuevo. Si actual no está en el
     *        contenedor no hace nada.
     */
    void reemplazar_widget(Widget& actual, Widget& nuevo);

    /**
     * \brief Devuelve el alto del contenedor.
     * 
     * El alto de una caja vertical es el alto del padre de la misma.
     */
    int obtener_alto() const override;

    /**
     * \brief Devuelve el ancho del contenedor.
     * 
     * El ancho de una caja vertical se determina como el ancho del hijo más
     * ancho que fue agregado. Si no se agregó ningún hijo devuelve 0.
     */
    int obtener_ancho() const override;

    /**
     * \brief Renderiza el widget.
     * 
     * Renderiza todos los hijos agregados al widget.
     */
    void renderizar(Ventana& ventana, const Posicion& punto) override;

protected:
    virtual std::vector<std::pair<Posicion, Widget*>>
        obtener_widgets(const Posicion& punto);
    
    virtual std::vector<Widget*> obtener_widgets();

private:
    std::list<Widget*> arriba;
    std::list<Widget*> abajo;
    int alto, ancho;
    int px, py;
};

} // namespace cliente

#endif // _CAJA_VERTICAL_H_
