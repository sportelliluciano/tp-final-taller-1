#ifndef _I_NOTIFICABLE_H_
#define _I_NOTIFICABLE_H_

#include "cliente/video/teclas.h"
#include "cliente/video/posicion.h"

namespace cliente {

/**
 * \brief Interfaz notificable. Permite ser notificado de eventos.
 */
class INotificable {
public:
    /**
     * \brief Se ejecuta cuando el usuario hace clic con el boton izquierdo
     *        del mouse sobre el widget.
     * 
     * (x, y) punto relativo al widget donde se hizo clic.
     * 
     * Devuelve false si el evento debe dejar de propagarse.
     */
    virtual bool mouse_click_izquierdo(const Posicion& punto) = 0;
    
    /**
     * \brief Se ejecuta cuando el usuario hace clic con el boton derecho
     *        del mouse sobre el widget.
     * 
     * (x, y) punto relativo al widget donde se hizo clic.
     * 
     * Devuelve false si el evento debe dejar de propagarse.
     */
    virtual bool mouse_click_derecho(const Posicion& punto) = 0;

    /**
     * \brief Se ejecuta cuando el usuario hace scroll con el mouse teniendo
     *        el puntero sobre el widget.
     * 
     * (x, y) punto relativo al widget donde se hizo scroll.
     * dy: positivo si el scroll es hacia arriba, negativo en caso contrario.
     * 
     * Devuelve false si el evento debe dejar de propagarse.
     */
    virtual bool mouse_scroll(const Posicion& punto, int dy) = 0;

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
    virtual bool mouse_inicio_arrastre(const Posicion& punto) = 0;

    /**
     * \brief Se ejecuta cuando el usuario mueve el mouse sobre la ventana.
     * 
     * (x, y) coordenadas del mouse.
     */
    virtual bool mouse_movimiento(const Posicion& punto) = 0;

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
    virtual bool mouse_fin_arrastre(const Posicion& punto) = 0;

    /**
     * \brief Se ejecuta cuando el usuario presiona una tecla.
     */
    virtual bool teclado_presionado(tecla_t tecla) = 0;

    /**
     * \brief Se ejecuta cuando el usuario suelta una tecla.
     */
    virtual bool teclado_suelto(tecla_t tecla) = 0;

    /**
     * \brief Se ejecuta cuando el mouse entra en el área del widget.
     */
    virtual bool mouse_entra(const Posicion& punto) = 0;

    /**
     * \brief Se ejecuta cuando el mouse sale del área del widget.
     */
    virtual bool mouse_sale(const Posicion& punto) = 0;

    /**
     * \brief Este evento se ejecuta cuando el usuario decide cerrar la ventana.
     */
    virtual bool cerrar_ventana() = 0;
};

} // namespace cliente

#endif // _I_NOTIFICABLE_H_