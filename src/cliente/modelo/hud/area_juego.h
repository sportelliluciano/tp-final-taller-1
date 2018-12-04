#ifndef _AREA_JUEGO_H_
#define _AREA_JUEGO_H_

#include <unordered_set>

#include "cliente/modelo/juego.h"
#include "cliente/modelo/hud/tostador.h"
#include "cliente/video/camara.h"
#include "cliente/video/posicion.h"
#include "cliente/video/widgets/widget.h"
#include "cliente/red/servidor.h"

namespace cliente {

/**
 * \brief Área de juego.
 * 
 * El área de juego es el controlador principal del juego. El mismo se encarga
 * de comunicar al servidor toda la interacción del usuario.
 */
class AreaJuego : public Widget {
public:
    /**
     * \brief Crea un nuevo área de juego.
     */
    AreaJuego(Juego& juego_, Servidor& servidor_, Tostador& tostador_);

    /**
     * \brief Centra la cámara en el centro de construcciones del jugador.
     */
    void centrar_camara();

    /**
     * \brief Activa el modo de venta de edificios.
     */
    void set_modo_vender(bool habilitado);

    /**
     * \brief Configura el tamaño del área de juego.
     */
    void set_tamanio(int ancho_, int alto_);

    /**
     * \brief Indica que hay un edificio a ser ubicado.
     */
    void ubicar_edificio(const Edificio* edificio);

    /** Interfaz widget **/
    virtual int obtener_alto() const override;
    virtual int obtener_ancho() const override;
    virtual void renderizar(Ventana& ventana, const Posicion& punto) override;
    virtual bool mouse_click_izquierdo(const Posicion& punto) override;
    virtual bool mouse_click_derecho(const Posicion& punto) override;
    virtual bool mouse_inicio_arrastre(const Posicion& punto) override;
    virtual bool mouse_movimiento(const Posicion& punto) override;
    virtual bool mouse_fin_arrastre(const Posicion& punto) override;
    virtual bool mouse_entra(const Posicion& punto) override;
    virtual bool mouse_sale(const Posicion& punto) override;
    virtual bool teclado_presionado(tecla_t tecla) override;
    virtual bool teclado_suelto(tecla_t tecla) override;

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

    bool en_modo_vender = false;

    Posicion mouse;
    SpriteAnimado* sprite_mouse = nullptr;

    SpriteAnimado mouse_atacar;
    SpriteAnimado mouse_vender;
    SpriteAnimado mouse_mover_tropa;

    int edificio_seleccionado;
    bool hay_edificio_seleccionado = false;
    std::unordered_set<int> unidades_seleccionadas;

    const Edificio* edificio_a_ubicar = nullptr;

    bool seleccionar_edificio(const Posicion& punto);
    void deseleccionar_edificio();
    bool seleccionar_tropas(int x0, int y0, int x1, int y1);
    void deseleccionar_tropas();
    void seleccionar(const Rectangulo& area);

    void renderizar_edificio_ubicando(Ventana& ventana);
    void renderizar_cursor(Ventana& ventana);
    void renderizar_rectangulo_seleccion(Ventana& ventana);
};

} // namespace cliente

#endif // _AREA_JUEGO_H_
