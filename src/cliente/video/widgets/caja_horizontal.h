#ifndef _CAJA_HORIZONTAL_H_
#define _CAJA_HORIZONTAL_H_

#include <list>
#include <functional>
#include <vector>
#include <utility>

#include "cliente/video/widgets/contenedor.h"

namespace cliente {

/**
 * \brief Caja con acomodamiento automático horizontal
 * 
 * Los elementos se acomodan uno al lado del otro, horizontalmente.
 */
class CajaHorizontal : public Contenedor {
public:
    CajaHorizontal(int x_, int y_, int ancho_, int alto_);

    /**
     * \brief Agrega al hijo al principio del contenedor.
     * 
     * El nuevo hijo quedará contra el principio del mismo.
     */
    void empaquetar_al_frente(Widget& hijo);

    /**
     * \brief Agrega un nuevo hijo empaquetado al final del contenedor.
     * 
     * El nuevo hijo quedará contra el final del mismo.
     */
    void empaquetar_al_fondo(Widget& hijo);

    /**
     * \brief Devuelve el alto del contenedor.
     * 
     * El alto del mismo se determina como el alto del hijo más alto que fue
     * añadido al contenedor. De no haber hijos devolverá 0.
     */
    int obtener_alto() const override;

    /**
     * \brief Devuelve el ancho del contenedor.
     * 
     * El ancho de una caja horizontal es el ancho del padre.
     */
    int obtener_ancho() const override;

    /**
     * \brief Renderiza el widget.
     * 
     * Renderiza todos los hijos agregados al widget.
     */
    void renderizar(Ventana& ventana, const Posicion& punto) override;

protected:
    virtual std::vector<Widget*> obtener_widgets();
    
    virtual std::vector<std::pair<Posicion, Widget*>>
        obtener_widgets(const Posicion& punto);

private:
    std::list<Widget*> frente;
    std::list<Widget*> fondo;
    int alto, ancho;
    int px, py;
};

} // namespace cliente

#endif // _CAJA_HORIZONTAL_H_
