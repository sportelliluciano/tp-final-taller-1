#ifndef _CAJA_HORIZONTAL_H_
#define _CAJA_HORIZONTAL_H_

#include <list>

#include "cliente/video/widgets/widget.h"

namespace cliente {

/**
 * \brief Caja con acomodamiento automático horizontal
 * 
 * Los elementos se acomodan uno al lado del otro, horizontalmente.
 */
class CajaHorizontal : public Widget {
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
    void renderizar(Ventana& ventana, int x, int y) override;

    /**
     * \brief Se ejecuta cuando el usuario hace clic con el boton izquierdo
     *        del mouse sobre el widget.
     * 
     * (x, y) punto relativo al widget donde se hizo clic.
     * 
     * Devuelve true si el evento debe dejar de propagarse.
     */
    virtual bool mouse_click_izquierdo(int x, int y);
    
    /**
     * \brief Se ejecuta cuando el usuario hace clic con el boton derecho
     *        del mouse sobre el widget.
     * 
     * (x, y) punto relativo al widget donde se hizo clic.
     * 
     * Devuelve true si el evento debe dejar de propagarse.
     */
    virtual bool mouse_click_derecho(int x, int y);

    /**
     * \brief Se ejecuta cuando el usuario hace scroll con el mouse teniendo
     *        el puntero sobre el widget.
     * 
     * (x, y) punto relativo al widget donde se hizo scroll.
     * dy: positivo si el scroll es hacia arriba, negativo en caso contrario.
     * 
     * Devuelve true si el evento debe dejar de propagarse.
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

private:
    std::list<Widget*> frente;
    std::list<Widget*> fondo;
    int alto, ancho;
    int px, py;

    Widget* ultimo_notificado_movimiento = nullptr;

    void buscar_por_x(int x, std::function<void(Widget&, int)> callback);
};

} // namespace cliente

#endif // _CAJA_HORIZONTAL_H_
