#ifndef _BOTON_TEMPORIZADO_H_
#define _BOTON_TEMPORIZADO_H_

#include "cliente/modelo/hud/tooltip.h"
#include "cliente/video/color.h"
#include "cliente/video/ventana.h"
#include "cliente/video/widgets/widget.h"

namespace cliente {

/**
 * \brief Boton con temporizador y cola
 */
class BotonTemporizado : public Widget {
public:
    BotonTemporizado(int sprite);

    /**
     * \brief Devuelve el ancho del botón.
     */
    int obtener_alto() const override;
    
    /**
     * \brief Devuelve el ancho del botón.
     */
    int obtener_ancho() const override;

    /**
     * \brief Renderiza el botón en la posición indicada.
     */
    virtual void renderizar(Ventana& ventana, const Posicion& punto) override;

    /**
     * \brief Setea la cola a mostrar en el botón. Sólo se mostrará si el 
     *        número es mayor a 0.
     */
    void set_cola(int cantidad);

    /**
     * \brief Inicia la cuenta regresiva en la cantidad de segundos indicada.
     *        Se detendrá al llegar a 0.
     */
    void set_tiempo(int segundos);

    /**
     * \brief Habilita o deshabilita el filtro del boton.
     */
    void set_filtro(bool gris, bool verde);

    /**
     * \brief Reinicia el botón a su estado original.
     */
    void reiniciar();

    /**
     * \brief Muestra el tooltip cuando el mouse esta sobre el botón.
     */
    bool mouse_entra(const Posicion& pos) override;

    /**
     * \brief Oculta el tooltip cuando el mouse sale del botón.
     */
    bool mouse_sale(const Posicion& pos) override;

protected:
    Tooltip tooltip;

private:
    bool mostrar_tooltip = false;
    int cola = 0;
    int segundos_restantes = 0;
    bool con_filtro_gris = false;
    bool con_filtro_verde = false;
    int sprite_id = -1;

    /**
     * \brief Obtiene un filtro semi-transparente para agregarle al botón.
     */
    Textura& obtener_filtro(Ventana& ventana, const Color& color);

    /**
     * \brief Renderiza el filtro correspondiente sobre la ventana.
     */
    void renderizar_filtros(Ventana& ventana, const Posicion& punto);

    /**
     * \brief Renderiza la cantidad de elementos en cola.
     */
    void renderizar_cola(Ventana& ventana, const Posicion& punto);

    /**
     * \brief Renderiza el tiempo restante sobre el boton.
     */
    void renderizar_tiempo_restante(Ventana& ventana, const Posicion& punto);
};

} // namespace cliente

#endif // _BOTON_TEMPORIZADO_H_
