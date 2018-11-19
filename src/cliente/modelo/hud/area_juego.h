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
    virtual void renderizar(Ventana& ventana, const Posicion& punto) override;

    /**
     * \brief Se ejecuta cuando el usuario hace clic con el botón izquierdo.
     */
    virtual bool mouse_click_izquierdo(const Posicion& punto) override;

    /**
     * \brief Se ejecuta cuando el usuario hace clic con el botón derecho.
     */
    virtual bool mouse_click_derecho(const Posicion& punto) override;

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
    virtual bool mouse_inicio_arrastre(const Posicion& punto) override;

    /**
     * \brief Se ejecuta cuando el usuario mueve el mouse sobre la ventana.
     * 
     * (x, y) coordenadas del mouse.
     */
    virtual bool mouse_movimiento(const Posicion& punto) override;

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
    virtual bool mouse_fin_arrastre(const Posicion& punto) override;

    virtual bool mouse_entra(const Posicion& punto) override;
    virtual bool mouse_sale(const Posicion& punto) override;

    virtual bool teclado_presionado(tecla_t tecla) override;
    virtual bool teclado_suelto(tecla_t tecla) override;

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

    Posicion drag_start;
    Posicion drag_end;

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

    bool seleccionar_edificio(const Posicion& punto);
    void deseleccionar_edificio();
    bool seleccionar_tropas(int x0, int y0, int x1, int y1);
    void deseleccionar_tropas();

    const Edificio* edificio_a_ubicar = nullptr;
};

} // namespace cliente

#endif // _AREA_JUEGO_H_
