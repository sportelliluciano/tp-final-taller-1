#ifndef _CONTENEDOR_H_
#define _CONTENEDOR_H_

#include <vector>
#include <utility>

#include "cliente/video/widgets/widget.h"

namespace cliente {

/**
 * \brief Widget contenedor de otros widgets.
 * 
 * Actúa como un proxy de eventos.
 */
class Contenedor : public Widget {
protected:
    /**
     * \brief Devuelve todos los widgets contenidos.
     * 
     * Este método se utiliza para poder retransmitir los eventos de teclado y
     * todos aquellos eventos que sean agnósticos a la posición.
     */
    virtual std::vector<Widget*> obtener_widgets() = 0;

    /**
     * \brief Obtiene los widgets que están en la posición (x, y).
     * 
     * Este método se utiliza para determinar a qué widgets del contenedor se
     * le propagaran los eventos de mouse.
     * 
     * El valor devuelto es un par por cada widget que corresponda emitirle
     * el evento donde el primer elemento del par es la posición a pasarle
     * al widget y el segundo es el widget en sí.
     * 
     * Si este método no es sobreescrito por la clase hija, por defecto 
     * devolverá lo mismo que obtener_widgets() tomando como posición 
     * desplazada el mismo punto.
     */
    virtual std::vector<std::pair<Posicion, Widget*>>
        obtener_widgets(const Posicion& punto);

public:
    /**
     * \brief Se ejecuta cuando el usuario hace clic con el boton izquierdo
     *        del mouse sobre el widget.
     * 
     * (x, y) punto relativo al widget donde se hizo clic.
     * 
     * Devuelve true si el evento debe dejar de propagarse.
     */
    virtual bool mouse_click_izquierdo(const Posicion& punto);
    
    /**
     * \brief Se ejecuta cuando el usuario hace clic con el boton derecho
     *        del mouse sobre el widget.
     * 
     * (x, y) punto relativo al widget donde se hizo clic.
     * 
     * Devuelve true si el evento debe dejar de propagarse.
     */
    virtual bool mouse_click_derecho(const Posicion& punto);

    /**
     * \brief Se ejecuta cuando el usuario hace scroll con el mouse teniendo
     *        el puntero sobre el widget.
     * 
     * (x, y) punto relativo al widget donde se hizo scroll.
     * dy: positivo si el scroll es hacia arriba, negativo en caso contrario.
     * 
     * Devuelve true si el evento debe dejar de propagarse.
     */
    virtual bool mouse_scroll(const Posicion& punto, int dy);

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
    virtual bool mouse_inicio_arrastre(const Posicion& punto);

    /**
     * \brief Se ejecuta cuando el usuario mueve el mouse sobre la ventana.
     * 
     * (x, y) coordenadas del mouse.
     */
    virtual bool mouse_movimiento(const Posicion& punto);

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
    virtual bool mouse_fin_arrastre(const Posicion& punto);

    /**
     * \brief Se ejecuta cuando el mouse entra en el área del widget.
     */
    virtual bool mouse_entra(const Posicion& punto);

    /**
     * \brief Se ejecuta cuando el mouse sale del área del widget.
     */
    virtual bool mouse_sale(const Posicion& punto);

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

protected:
    /**
     * \brief Ultimo widget notificado de evento de entrada / salida.
     */
    Widget* ultimo_notificado_e_s = nullptr;
};

} // namespace cliente

#endif // _CONTENEDOR_H_
