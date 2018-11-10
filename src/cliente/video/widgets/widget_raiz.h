#ifndef _WIDGET_RAIZ_H_
#define _WIDGET_RAIZ_H_

#include "cliente/video/widgets/widget.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Este widget encapsula las métricas de la ventana y sólo propaga
 *        eventos hacia abajo.
 * 
 * Este es un widget especial ya que no tiene padre.
 */
class WidgetRaiz : public Widget {
public:
    WidgetRaiz(int ancho_ventana, int alto_ventana);

    virtual Widget* obtener_padre();

    virtual void set_padre(Widget* nuevo_padre);

    virtual int obtener_alto() const override;

    virtual int obtener_ancho() const override;

    virtual void renderizar(Ventana& ventana, int x, int y) override;

    virtual bool mouse_click_izquierdo(int x, int y) override;
    
    virtual bool mouse_click_derecho(int x, int y) override;

    virtual bool mouse_scroll(int x, int y, int dy) override;

    /**
     * \brief Se ejecuta cuando el usuario arrastra el mouse mientras hace
     *        clic con el boton izquierdo del mismo.
     * 
     * Este evento se ejecuta si el usuario creó un rectángulo arrastrando
     * con el mouse de al menos 4px x 4px. Un rectángulo menor a este se 
     * considera un clic normal.
     * 
     * (x, y) coordenadas de la primer esquina del rectángulo de selección
     * 
     * Devuelve false si el evento debe dejar de propagarse.
     */
    virtual bool mouse_inicio_arrastre(int x, int y);

    /**
     * \brief Se ejecuta cuando el usuario mueve el mouse sobre la ventana.
     * 
     * (x, y) coordenadas del mouse.
     */
    virtual bool mouse_movimiento(int x, int y);

    /**
     * \brief Se ejecuta cuando el usuario suelta el mouse luego de haber 
     *        arrastrado hasta formar un rectángulo con el botón izquierdo.
     * 
     * Este evento se ejecuta si el usuario creó un rectángulo arrastrando
     * con el mouse de al menos 4px x 4px. Un rectángulo menor a este se 
     * considera un clic normal.
     * 
     * (x, y) coordenadas de la segunda esquina del rectángulo de selección
     * 
     * Devuelve false si el evento debe dejar de propagarse.
     */
    virtual bool mouse_fin_arrastre(int x, int y);

    /**
     * \brief Se ejecuta cuando el mouse entra en el área del widget.
     */
    virtual bool mouse_entra(int x, int y);

    /**
     * \brief Se ejecuta cuando el mouse sale del área del widget.
     */
    virtual bool mouse_sale(int x, int y);

    /**
     * \brief Setea el primer hijo. 
     * 
     * A este widget se le propagaran todos los eventos.
     * 
     * Setear el hijo reemplazará al anterior, si lo hubiera.
     */
    virtual void setear_hijo(Widget* widget);

    virtual ~WidgetRaiz();

private:
    Widget *hijo = nullptr;
    int ancho, alto;
};

} // namespace cliente

#endif // _WIDGET_RAIZ_H_
