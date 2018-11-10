#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "cliente/video/i_notificable.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Widget dibujable en la pantalla.
 */
class Widget : public INotificable {
public:
    /**
     * \brief Devuelve el alto del widget, en píxeles.
     */
    virtual int obtener_alto() const = 0;

    /**
     * \brief Devuelve el ancho del widget, en píxeles.
     */
    virtual int obtener_ancho() const = 0;

    /**
     * \brief Renderiza el widget en la posición (x, y).
     * 
     * Renderiza el widget en la posición (x, y) dada en píxeles, relativa a
     * la ventana (global).
     */
    virtual void renderizar(Ventana& ventana, int x, int y) = 0;

    /**
     * \brief Devuelve el padre del widget.
     */
    virtual Widget* obtener_padre();

    /**
     * \brief Setea el padre del widget.
     */
    virtual void set_padre(Widget* nuevo_padre);

    /**
     * \brief Se ejecuta cuando el usuario hace clic con el boton izquierdo
     *        del mouse sobre el widget.
     * 
     * (x, y) punto relativo al widget donde se hizo clic.
     * 
     * Devuelve false si el evento debe dejar de propagarse.
     */
    virtual bool mouse_click_izquierdo(int x, int y);
    
    /**
     * \brief Se ejecuta cuando el usuario hace clic con el boton derecho
     *        del mouse sobre el widget.
     * 
     * (x, y) punto relativo al widget donde se hizo clic.
     * 
     * Devuelve false si el evento debe dejar de propagarse.
     */
    virtual bool mouse_click_derecho(int x, int y);

    /**
     * \brief Se ejecuta cuando el usuario hace scroll con el mouse teniendo
     *        el puntero sobre el widget.
     * 
     * (x, y) punto relativo al widget donde se hizo scroll.
     * dy: positivo si el scroll es hacia arriba, negativo en caso contrario.
     * 
     * Devuelve false si el evento debe dejar de propagarse.
     */
    virtual bool mouse_scroll(int x, int y, int dy);

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
     * \brief Se ejecuta cuando el usuario presiona una tecla.
     */
    virtual bool teclado_presionado(tecla_t tecla);

    /**
     * \brief Se ejecuta cuando el usuario suelta una tecla.
     */
    virtual bool teclado_suelto(tecla_t tecla);

    /**
     * \brief Este evento toma lugar cuando se cierra la ventana.
     */
    virtual bool cerrar_ventana();

    /**
     * \brief Destructor virtual.
     */
    virtual ~Widget();

protected:
    Widget* padre;

};

} // namespace cliente

#endif // _WIDGET_H_
