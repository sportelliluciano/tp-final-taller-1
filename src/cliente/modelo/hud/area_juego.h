#ifndef _AREA_JUEGO_H_
#define _AREA_JUEGO_H_

#include "cliente/modelo/juego.h"
#include "cliente/video/widgets/widget.h"

namespace cliente {

class AreaJuego : public Widget {
public:
    AreaJuego(Juego& juego_);

    void set_tamanio(int ancho_, int alto_);

    virtual int obtener_alto() const override;

    /**
     * \brief Devuelve el ancho del widget, en píxeles.
     */
    virtual int obtener_ancho() const override;

    /**
     * \brief Renderiza el widget en la posición (x, y).
     * 
     * Renderiza el widget en la posición (x, y) dada en píxeles, relativa a
     * la ventana (global).
     */
    virtual void renderizar(Ventana& ventana, int x, int y) override;

    /**
     * \brief Se ejecuta cuando el usuario hace clic con el botón izquierdo.
     */
    virtual bool mouse_click_izquierdo(int x, int y);

    /**
     * \brief Se ejecuta cuando el usuario hace clic con el botón derecho.
     */
    virtual bool mouse_click_derecho(int x, int y);

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

    virtual bool mouse_entra(int x, int y);
    virtual bool mouse_sale(int x, int y);

private:
    int ancho = 0, alto = 0;
    Juego& juego;

    bool esta_draggeando = false;

    int drag_start_x, drag_start_y;
    int drag_end_x, drag_end_y;

    int mover_camara_x = 0;
    int mover_camara_y = 0;
    int delay_camara = 0;
};

} // namespace cliente

#endif // _AREA_JUEGO_H_

