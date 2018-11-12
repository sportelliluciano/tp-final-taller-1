#ifndef _BOTON_TEMPORIZADO_H_
#define _BOTON_TEMPORIZADO_H_

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

    virtual void pre_renderizar(Ventana& ventana, int x, int y);

    /**
     * \brief Renderiza el botón en la posición indicada.
     */
    void renderizar(Ventana& ventana, int x, int y) override;

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

private:
    int cola = 0;
    int segundos_restantes = 0;
    bool con_filtro_gris = false;
    bool con_filtro_verde = false;
    int sprite_id = -1;
};

} // namespace cliente

#endif // _BOTON_TEMPORIZADO_H_
