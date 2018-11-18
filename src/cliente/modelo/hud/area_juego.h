#ifndef _AREA_JUEGO_H_
#define _AREA_JUEGO_H_

#include <unordered_set>

#include "cliente/modelo/juego.h"
#include "cliente/modelo/hud/tostador.h"
#include "cliente/video/camara.h"
#include "cliente/video/posicion.h"
#include "cliente/video/widgets/widget.h"
#include "cliente/servidor.h"

namespace cliente {

class AreaJuego : public Widget {
public:
    AreaJuego(Juego& juego_, Servidor& servidor_, Tostador& tostador_);

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

    virtual bool teclado_presionado(tecla_t tecla);
    virtual bool teclado_suelto(tecla_t tecla);

    void set_modo_vender(bool habilitado);

    void ubicar_edificio(const Edificio* edificio);

private:
    int ancho = 0, alto = 0;
    Camara camara;
    Juego& juego;
    Ejercito& ejercito;
    Infraestructura& infraestructura;
    Servidor& servidor;
    Tostador& tostador;

    bool esta_draggeando = false;
    bool ctrl_presionado = false;

    int drag_start_x, drag_start_y;
    int drag_end_x, drag_end_y;

    int mover_camara_x = 0;
    int mover_camara_y = 0;
    int delay_camara = 0;

    bool mouse_en_ventana = false;

    /**
     * \brief Setear a true para habilitar el modo venta de edificios.
     */
    bool en_modo_vender = false;

    Posicion mouse;
    SpriteAnimado* sprite_mouse = nullptr;

    SpriteAnimado mouse_atacar;
    SpriteAnimado mouse_vender;
    SpriteAnimado mouse_mover_tropa;

    Edificio* edificio_seleccionado = nullptr;
    std::unordered_set<Tropa*> unidades_seleccionadas;

    bool seleccionar_edificio(int x, int y);
    bool seleccionar_tropas(int x0, int y0, int x1, int y1);

    const Edificio* edificio_a_ubicar = nullptr;
};

} // namespace cliente

#endif // _AREA_JUEGO_H_
