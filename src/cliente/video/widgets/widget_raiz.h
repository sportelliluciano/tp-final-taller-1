#ifndef _WIDGET_RAIZ_H_
#define _WIDGET_RAIZ_H_

#include "cliente/video/widgets/contenedor.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Este widget encapsula las métricas de la ventana y sólo propaga
 *        eventos hacia abajo.
 * 
 * Este es un widget especial ya que no tiene padre.
 */
class WidgetRaiz : public Contenedor {
public:
    WidgetRaiz(int ancho_ventana, int alto_ventana);

    virtual Widget* obtener_padre();

    virtual void set_padre(Widget* nuevo_padre);

    virtual int obtener_alto() const override;

    virtual int obtener_ancho() const override;

    virtual void renderizar(Ventana& ventana, const Posicion& punto) override;

    /**
     * \brief Setea el primer hijo. 
     * 
     * A este widget se le propagaran todos los eventos.
     * 
     * Setear el hijo reemplazará al anterior, si lo hubiera.
     */
    virtual void setear_hijo(Widget* widget);

    virtual ~WidgetRaiz();

protected:
    virtual std::vector<Widget*> obtener_widgets();

private:
    Widget *hijo = nullptr;
    int ancho, alto;
};

} // namespace cliente

#endif // _WIDGET_RAIZ_H_
